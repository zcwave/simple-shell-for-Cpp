#ifndef __TSH__H__
#define __TSH__H__   

/* 
 * tsh - A tiny shell program with job control
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include <string>








/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */

// /* Job states */
// #define UNDEF 0 /* undefined */
// #define FG 1    /* running in foreground */
// #define BG 2    /* running in background */
// #define ST 3    /* stopped */

enum JobState {
    UNDEF = 0, /* undefined */
    FG = 1,    /* running in foreground */
    BG = 2,    /* running in background */
    ST = 3     /* stopped */
};


/* 
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

struct job_t {              /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    JobState state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
};



/* Global variables */
extern char **environ;      /* defined in libc */
int nextjid = 1;            /* next job ID to allocate */
char sbuf[MAXLINE];         /* for composing sprintf messages */
struct job_t jobs[MAXJOBS]; /* The job list */
const std::string prompt = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
bool verbose = false;           /* if true, print additional output */
volatile sig_atomic_t FG_PID_GLOBALS;
/* End global variables */

// #define BLOCK(set, old_set) / 
//         Sigprocmask(SIG_BLOCK, &(set), &(old_set))
// #define BLOCK_NOT_SAVE_OLD_SET(set) /
//         Sigprocmask(SIG_BLOCK, &(set), NULL)
// #define UNBLOCK(old_set) /
//         Sigprocmask(SIG_SETMASK, &(old_set), NULL)

/* Function prototypes */

/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv); 
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs); 
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *jobs, pid_t pid); 
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid); 
int pid2jid(pid_t pid); 
void listjobs(struct job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

#endif // __TSH__H__