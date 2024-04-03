#pragma once  

/* 
 * tsh - A tiny shell program with job control
 */

#include "jobs_types.h"
#include <vector>
#include <iterator>
#include <sstream>
#include <cassert>
#include <sys/wait.h>
#include <iostream>


extern bool verbose; 

decltype(auto) parseline(auto&& cmdline, auto&& argv);
bool isbuiltinCommand(auto&& argv);
void eval(auto&& command);

/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */
bool isbuiltinCommand(auto&& argv) {

    std::string cmd_name = argv[0];
    if (cmd_name == "quit") // quit command.
        exit(0);
    else if (cmd_name == "&")   // Ignore singleton &. nothing is happen.
        return true;
    return false;               /* not a builtin command */
}



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
void eval(auto&& command) { 

    if (command.size() == 0) {
        return; // Ignore empty lines.
    }

    std::vector<const char *> argv{}; // argument list exec()
    // constructing argv[MAXARGS] And return true if job is bg. 
    // iow, the last char is &.

    auto state { parseline(command ,argv) }; 
        // should the job run in bg or fg?
 
    if (!isbuiltinCommand(argv)) {
        if (auto pid = fork()) {
            wait(0);
        }
        else {
        // child runs user job.
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
decltype(auto) parseline(auto&& cmdline, auto&& argv) {

    JobState state = FG;
    std::istringstream iss(cmdline);
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>{}};

    for (auto &x : tokens) {
        argv.push_back(std::move(x.c_str()));
    }

    /* should the job run in the background? */
    if (*argv.rbegin() == std::string("&")) {
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


// in "lib/utilities.h"
void usage(void);
