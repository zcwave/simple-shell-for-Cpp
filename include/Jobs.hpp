#pragma once

#include <vector>
#include <stdexcept>
#include "tsh.h"
#include "jobs_types.h"

class Jobs {
private:
    Jobs(int job_max_num) : 
                            job_list({}), 
                            fgpid(0), 
                            next_jid(1), 
                            max_jobs(job_max_num) 
    {
        if (job_max_num < 0) {
            throw std::logic_error("job_max_num must be a vaild number.");
        }
        job_list.reserve(job_max_num);
        for (auto &x : job_list) {
            x  = job_t();
        }
        //? 这是有必要的吗？

    }
    ~Jobs();

    Jobs(const Jobs & src) = delete;
    Jobs(Jobs && src) = delete;
    Jobs& operator=(const Jobs & rhs) = delete;
    Jobs& operator=(Jobs && rhs) = delete;

private: 
    std::vector<job_t> job_list;
    pid_t fgpid;
    int next_jid;
    int max_jobs;

public:
    static Jobs& getInstance() {
        static Jobs instance(MAXJOBS);
        return instance;
    }


    /* addjob - Add a job to the job list */
    bool addJob(pid_t pid, JobState state, char* cmdline);


    /* deletejob - Delete a job whose PID=pid from the job list */
    void deleteJob(pid_t pid);


    /* fgpid - Return PID of current foreground job, 0 if no such job */
    pid_t getFgPid() const {
        return fgpid;
    }
    job_t getJobByPid(pid_t pid);
    job_t getJobByJid(int jid);

    int pid2jid(pid_t pid); //! 这个函数应该是类内的吗？

    /* listjobs - Print the job list */
    void list();


};