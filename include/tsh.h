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
#include <string_view>
#include <vector>
#include <iostream>

#include "jobs.h"


/* Misc manifest constants */
constexpr int MAXLINE = 1024;     /* max line size */
constexpr int MAXARGS = 128;      /* max args on a command line */
constexpr int MAXJOBS = 16;       /* max jobs at any point in time */
constexpr int MAXJID  = 1 << 16;  /* max job ID : 65536 */ 







// #define BLOCK(set, old_set) / 
//         Sigprocmask(SIG_BLOCK, &(set), &(old_set))
// #define BLOCK_NOT_SAVE_OLD_SET(set) /
//         Sigprocmask(SIG_BLOCK, &(set), NULL)
// #define UNBLOCK(old_set) /
//         Sigprocmask(SIG_SETMASK, &(old_set), NULL)




// /* Function prototypes */

// /* Here are the functions that you will implement */
void eval(const std::string_view command_line);  
bool isbuiltinCommand(const std::string_view cmd_name);
// void do_bgfg(char **argv);
// void waitfg(pid_t pid);

// void sigchld_handler(int sig);
// void sigtstp_handler(int sig);
// void sigint_handler(int sig);

// /* Here are helper routines that we've provided for you */
// int parseline(const char *cmdline, char **argv); 
// void sigquit_handler(int sig);

// void clearjob(struct job_t *job);
// void initjobs(struct job_t *jobs);
// int maxjid(struct job_t *jobs); 
// int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
// int deletejob(struct job_t *jobs, pid_t pid); 
// pid_t fgpid(struct job_t *jobs);
// struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
// struct job_t *getjobjid(struct job_t *jobs, int jid); 
// int pid2jid(pid_t pid); 
// void listjobs(struct job_t *jobs);

void usage(void);
// void unix_error(char *msg);
void app_error(const std::string_view msg);
// typedef void handler_t(int);
// handler_t *Signal(int signum, handler_t *handler);

#endif // __TSH__H__