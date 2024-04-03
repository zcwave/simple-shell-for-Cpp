#include <iterator>
#include <sstream>
#include <cassert>
#include <sys/wait.h>
#include <iostream>

#include "tsh.h"

using std::vector, std::string;


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


decltype(auto) parseline(auto&& cmdline, auto&& argv) {

    JobState state = FG;
    std::istringstream iss(cmdline);
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                                    std::istream_iterator<std::string>{}};

    for (auto &x : tokens) {
        argv.push_back(std::move(x.c_str()));
    }

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
bool isbuiltinCommand(auto&& argv) {

    string cmd_name = argv[0];
    if (cmd_name == "quit") // quit command.
        exit(0);
    else if (cmd_name == "&")   // Ignore singleton &. nothing is happen.
        return true;
    return false;               /* not a builtin command */
}

