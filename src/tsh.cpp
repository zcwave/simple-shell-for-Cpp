#include "tsh.h"
#include <iterator>
#include <sstream>
#include <cassert>
#include "Jobs.hpp"

using std::vector;
using std::string;

#define BLOCK(set, old_set) sigprocmask(SIG_BLOCK, &(set), &(old_set))
#define BLOCK_NOT_SAVE_OLD_SET(set) sigprocmask(SIG_BLOCK, &(set), NULL)
#define UNBLOCK(old_set) sigprocmask(SIG_SETMASK, &(old_set), NULL)

/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
*/
void eval(const std::string &command) { 

    if (command.size() == 0) {
        return; // Ignore empty lines.
    }

    std::vector<const char *> argv{}; // argument list exec()
   // constructing argv[MAXARGS] And return true if job is bg. 
   // iow, the last char is &.
    auto state { parseline(command ,argv) }; 
    // should the job run in bg or fg?
 
    if (!isbuiltinCommand(argv)) {
 
        sigset_t mask_all;
        sigset_t mask_one, prev_one;
    
        sigfillset(&mask_all);

        sigemptyset(&mask_one);
        sigaddset(&mask_one, SIGCHLD);
        
        BLOCK(mask_one, prev_one); // Block SIGCHLD.
     
        if (auto pid = fork()) {

            BLOCK_NOT_SAVE_OLD_SET(mask_all);
            Jobs::getInstance().addJob(pid, state, command);
            UNBLOCK(prev_one); // Unblock SIGCHLD.
        
            /* Parent waits for fg job to terminate.*/
            if (state == FG){
                waitfg(pid); 
            }
            else {
                // the job is bg.
                BLOCK_NOT_SAVE_OLD_SET(mask_all);
                auto jid = Jobs::getInstance().pid2jid(pid); 
                if (jid.has_value())
                    printf("[%d] (%d) %s", 
                                      jid.value(), 
                                              pid, 
                                      command.c_str());
                else {
                    throw std::runtime_error("Not found jid form PID2JID().");
                }
            }  
            UNBLOCK(prev_one);//Unblock all signal
        }
        else {
        // child runs user job.
            UNBLOCK(prev_one); //Unblock SIGCHLD.

            setpgid(0, 0);
            if (execve(argv[0], 
                       const_cast<char * const *>(argv.data()), 
                       environ) < 0) 
            {
                std::cout << argv[0] << ": Command not found. " << std::endl;
                exit(0);
            }
        }
    }
    return;
}

/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */

JobState parseline(const string &cmdline, std::vector<const char*> &argv) {

    JobState state = FG; //? 默认是一个FG好吗？ 默认是UNDEF怎么样？
    std::istringstream iss(cmdline);
    std::vector<std::string> tokens {
        std::istream_iterator<std::string>{iss},
        std::istream_iterator<std::string>{}};

    for (auto &x : tokens) {
        argv.push_back(std::move(x.c_str()));
    }
    //! 待测试 move！ 与 auto &

    //? DEBUG: 
    if (verbose)
        std::cout << "Last argument in argv: " << *argv.rbegin() << std::endl;


    /* should the job run in the background? */
    if (*argv.rbegin() == string("&")) {
        if (argv.size() > 2) {
            argv.pop_back();
            state = BG;
        } else {
            return state; // single "&"
        }
    }
    argv.push_back(nullptr); // add NULL as argv END.
    return state;
}


/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */
bool isbuiltinCommand(std::vector<const char *> &argv) {

    string cmd_name = argv[0];
    if (cmd_name == "quit") // quit command.
        exit(0);
    else if (cmd_name == "&")   // Ignore singleton &. nothing is happen.
        return true;
    else if (cmd_name == "jobs") { // jobs command.
        sigset_t mask_all, prev_all;
        sigfillset(&mask_all);
    
        BLOCK(mask_all, prev_all);
        Jobs::getInstance().list();
        UNBLOCK(prev_all);

        return true;
    }
    else if (cmd_name == "bg" || cmd_name == "fg") { // bg or fg commands.
        do_bgfg(argv);
        return true;
    }
    return false;               /* not a builtin command */
}

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(std::vector<const char *> &argv) {
    int id;
    std::optional<job_t> job;
    sigset_t mask_all, prev_all;
    sigfillset(&mask_all);
 
    if (argv[1] == nullptr){
        assert(argv.size() == 2);
        // single fg or bg.
        printf("%s command requires PID or %%jobid argument\n", argv[0]);
        return;
    } else if (sscanf(argv[1], "%%%d", &id) > 0){
        BLOCK(mask_all, prev_all);
        job = Jobs::getInstance().getJobByJid(id);
        UNBLOCK(prev_all);

        if (!job.has_value()) {
            // not found the job.
            printf("%%%d: No such job\n", id);
            return;
        }

    } else if(sscanf(argv[1], "%d", &id) > 0){
        BLOCK(mask_all, prev_all);
        job = Jobs::getInstance().getJobByPid(id);
        UNBLOCK(prev_all); 
        if (!job.has_value()) {
 
            printf("(%d): No such process\n", id);
            return;
        }

   } else {
        // not pid or jid.
        printf("%s: argument must be a PID or %%jobid\n", argv[0]);
        return;
   }
 
    if (argv[0] == string("bg")) {

        kill(-(job.value().pid), SIGCONT);
        job.value().state = BG;
        printf("[%d] (%d) %s", 
                        job.value().jid, 
                        job.value().pid, 
                        job.value().cmdline.c_str());

    } else if (argv[0] == string("fg")) {

        kill(-(job->pid), SIGCONT);
        job.value().state = FG;
        waitfg(job.value().pid);

    }
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid) {
   sigset_t mask;
   sigemptyset(&mask);
 
   FG_PID_GLOBALS = 0;
   while (!FG_PID_GLOBALS)
        sigsuspend(&mask);
}






