#include "tsh.h"
#include "jobs_types.h"
#include <algorithm>
/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

// /* clearjob - Clear the entries in a job struct */
// void clearjob(job_t *job) {
//     job->pid = 0;
//     job->jid = 0;
//     job->state = UNDEF;
//     job->cmdline[0] = '\0';
// }

// /* initjobs - Initialize the job list */
// void initjobs(struct job_t *jobs) {
//     int i;

//     for (i = 0; i < MAXJOBS; i++)
// 		clearjob(&jobs[i]);
// }

/* maxjid - Returns largest allocated job ID */
int maxjid(const std::vector<job_t> &jobs) {
	auto comp = [](job_t j1, job_t j2) { return j1.jid < j2.jid;};
	auto max = std::max_element(jobs.begin(), jobs.end(), comp);
    return max->jid;
}

bool addjob(std::vector<job_t> &jobs, pid_t pid, JobState state, char *cmdline) 
{
    int i;
    
    if (pid < 1)
		return false;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == 0) {
	    jobs[i].pid = pid;
	    jobs[i].state = state;
	    jobs[i].jid = nextjid++;
	    if (nextjid > MAXJOBS)
		nextjid = 1;
	    strcpy(jobs[i].cmdline, cmdline);
  	    if(verbose){
	        printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
            }
            return 1;
	}
    }
    printf("Tried to create too many jobs\n");
    return 0;
}

int deletejob(struct job_t *jobs, pid_t pid) 
{
    int i;

    if (pid < 1)
	return 0;

    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == pid) {
	    clearjob(&jobs[i]);
	    nextjid = maxjid(jobs)+1;
	    return 1;
	}
    }
    return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs) {
    int i;

    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].state == FG)
	    return jobs[i].pid;
    return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid) {
    int i;

    if (pid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid)
	    return &jobs[i];
    return NULL;
}

struct job_t *getjobjid(struct job_t *jobs, int jid) 
{
    int i;

    if (jid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid == jid)
	    return &jobs[i];
    return NULL;
}

int pid2jid(pid_t pid) 
{
    int i;

    if (pid < 1)
	return 0;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid) {
            return jobs[i].jid;
        }
    return 0;
}


/******************************
 * end job list helper routines
 ******************************/