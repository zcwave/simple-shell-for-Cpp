#include "tsh.h"
#include <iterator>
#include <sstream>
#include <cassert>
#include "Jobs.hpp"

using std::vector;
using std::string;
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
   // constructing argv[MAXARGS] And return true if job is bg. iow, the last char is &.
    auto state { parseline(command ,argv) }; // should the job run in bg or fg?
 
    if (!isbuiltinCommand(argv)) {
 
    //  sigset_t mask_all;
    //  sigset_t mask_one, prev_one;
 
    //  sigfillset(&mask_all);

    //  sigemptyset(&mask_one);
    //  sigaddset(&mask_one, SIGCHLD);
     
     // BLOCK(mask_one, prev_one); // Block SIGCHLD.
     
        if (auto pid = fork()) {
            Jobs::getInstance().addJob(pid, state, command);
            
            wait(0);
            // BLOCK_NOT_SAVE_OLD_SET(mask_all);
            // addjob(jobs, pid, state, cmdline);
            // UNBLOCK(prev_one); // Unblock SIGCHLD.
        
            /* Parent waits for fg job to terminate.*/
            // if (state == FG){
            //     waitfg(pid);
            // }
            // else {
            //     // the job is bg.
            //     // BLOCK_NOT_SAVE_OLD_SET(mask_all);
            //     int jid = Jobs::getInstance().pid2jid(pid);
            //     printf("[%d] (%d) %s", jid, pid, command.c_str());
            // }  
            // // UNBLOCK(prev_one);//Unblock all signal
        }
        else {
        // child runs user job.
        // UNBLOCK(prev_one); //Unblock SIGCHLD.
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

    JobState state = FG;
    std::istringstream iss(cmdline);
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>{}};

    for (auto &x : tokens) {
        argv.push_back(std::move(x.c_str()));
    }
    //! 待测试 move！ 与 auto &

    //? DEBUG: std::cout << *argv.rbegin() << std::endl;
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
bool isbuiltinCommand(const std::vector<const char *> argv) {

    string cmd_name = argv[0];
    if (cmd_name == "quit") // quit command.
        exit(0);
    else if (cmd_name == "&")   // Ignore singleton &. nothing is happen.
        return true;
    else if (cmd_name == "jobs") { // jobs command.
//      sigset_t mask_all, prev_all;
//      Sigfillset(&mask_all);
 
//      BLOCK(mask_all, prev_all);
        Jobs::getInstance().list();
//      UNBLOCK(prev_all);
        return true;
    }
    else if (cmd_name == "bg" || cmd_name == "fg") { // bg or fg commands.
        // do_bgfg(argv);
        return true;
    }
    return false;               /* not a builtin command */
}

// /* 
//  * do_bgfg - Execute the builtin bg and fg commands
//  */
// void do_bgfg(char **argv) 
//  {
//    struct job_t *job;
//    int id;
//    sigset_t mask_all, prev_all;
//    Sigfillset(&mask_all);
 
//    if (argv[1] == NULL){
//      // single fg or bg.
//      printf("%s command requires PID or %%jobid argument\n", argv[0]);
//      return;
//    }
//    else if (sscanf(argv[1], "%%%d", &id) > 0){
//      BLOCK(mask_all, prev_all);
//      job = getjobjid(jobs, id);
//      UNBLOCK(prev_all);
 
//      if (job == NULL){
//        // not found the job.
//        printf("%%%d: No such job\n", id);
//        return;
//      }
//    }
//    else if(sscanf(argv[1], "%d", &id) > 0){
//      BLOCK(mask_all, prev_all);
//      job = getjobpid(jobs, id);
//      UNBLOCK(prev_all); 
 
//      if (job == NULL){
//        printf("(%d): No such process\n", id);
//        return;
//      }
//    }
//    else{
//      // not pid or jid.
//      printf("%s: argument must be a PID or %%jobid\n", argv[0]);
//      return;
//    }
 
//    if (!strcmp(argv[0], "bg")){
//      kill(-(job->pid), SIGCONT);
//      job->state = BG;
//      printf("[%d] (%d) %s", job->jid, job->pid, job->cmdline);
//    }
//    else if (!strcmp(argv[0], "fg")){
//      kill(-(job->pid), SIGCONT);
//      job->state = FG;
//      waitfg(job->pid);
//    }
//  }

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






