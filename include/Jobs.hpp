#pragma once

#include <vector>
#include <stdexcept>
#include "tsh.h"
#include "jobs_types.h"
#include <optional>

class Jobs {
private:
    Jobs(int job_max_num) : 
                            job_list({}), 
                            fgpid(0), 
                            next_jid(1), 
                            max_jobs(job_max_num) {
                                
        if (job_max_num < 0) {
            throw std::logic_error("job_max_num must be a vaild number.");
        }


        // init job_list.
        job_list.resize(job_max_num);
        //! The reverse method has no effect here.
    }

    ~Jobs(){}

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
    bool addJob(pid_t pid, JobState state, std::string cmdline);


    /* deletejob - Delete a job whose PID=pid from the job list */
    bool deleteJob(pid_t pid);


    /* maxjid - Returns largest allocated job ID */
    int maxjid() const;

    /* fgpid - Return PID of current foreground job, 0 if no such job */
    pid_t getFgPid() const {
        return fgpid;
    }


    /* getjobjid  - Find a job (by JID) on the job list */
    std::optional<job_t> getJobByPid(pid_t pid);
    

    std::optional<job_t> getJobByJid(int jid);


    /* pid2jid - Map process ID to job ID */
    int pid2jid(pid_t pid);     
    
    
    /* listjobs - Print the job list */
    void list();
};