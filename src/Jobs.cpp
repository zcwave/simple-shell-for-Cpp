#include "Jobs.hpp"
#include <iostream>
#include <algorithm>
#include <cassert>

bool Jobs::addJob(pid_t pid, JobState state, std::string cmdline) {
    
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

int Jobs::maxjid() const {
	auto comp = [](job_t j1, job_t j2) { return j1.jid < j2.jid;};
	auto max = std::max_element(job_list.begin(), job_list.end(), comp);
    return max->jid;
}

bool Jobs::deleteJob(pid_t pid) {
    auto clear = [](auto &job) {
        job_t tJob; //? 空的构造函数

        assert(tJob.pid == 0);
        assert(tJob.jid == 0);
        assert(tJob.state == UNDEF);
        assert(tJob.cmdline == "");

        job = tJob;  //? 会触发值拷贝吗？
    };

    if (pid < 1) {
        //! throw !!!
        return false;
    }
    
    auto cond = [&pid](job_t &x) {
        return x.pid == pid;
    };

    auto found = find_if(job_list.begin(), 
                         job_list.end(),
                         cond);

    if (found != job_list.end()) { // found it;
        auto &it = *found;
        clear(it);
        next_jid = maxjid() + 1;
        return true;
    }

    return 0;

}

std::optional<job_t> Jobs::getJobByPid(pid_t pid) {
    
    if (pid < 1)
        return std::nullopt;

    auto cond = [&pid](job_t j) {
        return j.pid == pid;
    };
    
    auto found = std::find_if(job_list.begin(), 
                              job_list.end(), 
                              cond);
    if (found != job_list.end()) 
        return *found;
    return std::nullopt;
}
    

std::optional<job_t> Jobs::getJobByJid(int jid) {

    auto cond = [&jid](job_t j) {
        return j.pid == jid;
    };
    
    auto found = std::find_if(job_list.begin(), 
                              job_list.end(), 
                              cond);
    if (found != job_list.end()) 
        return *found;
    return std::nullopt;
    
}


std::optional<int> Jobs::pid2jid(pid_t pid) {

    auto job = getJobByPid(pid);
    if (job.has_value()) {
        return job.value().jid;
    }
    return std::nullopt;

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

            printf("%s\n", x.cmdline.c_str());

	    }
    }


}