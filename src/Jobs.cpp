#include "Jobs.hpp"
#include <iostream>

bool Jobs::addJob(pid_t pid, JobState state, std::string cmdline) {
    int i;
    
    if (pid < 1)
		return false;


    for (auto &x : job_list) {
        if (x.pid == 0) {
            x = job_t(pid, next_jid++, state, cmdline);

            if (next_jid > MAXJOBS)
                next_jid = 1;

            if (state == FG) {
                fgpid = pid;
            }

            if (verbose) {

                printf("Added job [%d] (%d) %s\n", 
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





void Jobs::list() {
    for (auto x : job_list) {
        if (x.pid != 0) {
            printf("[%d] (%d) ", x.jid, x.pid);
            switch (x.state) {
            case BG: 
                printf("Running ");
                break;
            case FG: 
                printf("Foreground ");
                break;
            case ST: 
                printf("Stopped ");
                break;
            default:
                printf("listjobs: Internal error: job[%d].state=%d ", 
                            x.jid, 
                            x.state);
            }

            printf("%s", x.cmdline.c_str());

	    }
    }


}