#pragma once

#include <vector>
#include "jobs_types.h"
#include "tsh.h"

class Jobs {
private:
    Jobs();
    ~Jobs();

    Jobs(const Jobs & src) = delete;
    Jobs(Jobs && src) = delete;
    Jobs& operator=(const Jobs & rhs) = delete;
    Jobs& operator=(Jobs && rhs) = delete;

private: 
    // static const int MAXJOBS = 16;
    std::vector<job_t> job_list(MAXJOBS);
    pid_t fgpid = 0;

public:
    static Jobs& getInstance() {
        static Jobs instance;
        return instance;
    }


    /* addjob - Add a job to the job list */
    bool addJob(pid_t pid, JobState state, char *cmdline);


    /* deletejob - Delete a job whose PID=pid from the job list */
    void deleteJob(pid_t pid);

    job_t getJobByPid(pid_t pid);
    job_t getJobByJid(int jid);

    int pid2jid(pid_t pid); //! 这个函数应该是类内的吗？

    /* listjobs - Print the job list */
    void list();


};