#ifndef __JOBS__H__
#define __JOBS__H__


// /* Job states */
// #define UNDEF 0 /* undefined */
// #define FG 1    /* running in foreground */
// #define BG 2    /* running in background */
// #define ST 3    /* stopped */
#include <string>
#include <sys/types.h>
enum JobState {    /* 
    * Jobs states: FG (foreground), BG (background), ST (stopped)
    * Job state transitions and enabling actions:
    *     FG -> ST  : ctrl-z
    *     ST -> FG  : fg command
    *     ST -> BG  : bg command
    *     BG -> FG  : fg command
    * At most 1 job can be in the FG state.
    */
    UNDEF = 0, /* undefined */
    FG = 1,    /* running in foreground */
    BG = 2,    /* running in background */
    ST = 3     /* stopped */
};


struct job_t {              /* The job struct */
    job_t(pid_t p, int j, JobState js, std::string cl) : 
                                    pid(p), jid(j), state(js), cmdline(cl) {}

    job_t() : job_t(0, 0, UNDEF, {}) {}
    
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    JobState state;         /* UNDEF, BG, FG, or ST */
    // char cmdline[MAXLINE];  /* command line */
    std::string cmdline{};
};


#endif // __JOBS__H__