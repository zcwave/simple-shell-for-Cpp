#include "Jobs.hpp"
#include <iostream>

bool Jobs::addJob(pid_t pid, JobState state, char* cmdline) {
    int i;
    
    if (pid < 1)
		return false;


    for (auto &x : job_list) {
        if (x.pid == 0) {
            x = job_t(pid, next_jid++, state, cmdline);
            if (next_jid > MAXJOBS)
                next_jid = 1;
            if (verbose) {
                printf("Added job [%d] %d %s\n", 
                                    x.jid, 
                                    x.pid, 
                                    x.cmdline.c_str());
            }
            return true;
       }
    }
    printf("Tried to create too many jobs\n");

    return false;
}