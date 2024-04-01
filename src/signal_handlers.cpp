#include "tsh.h"
#include "jobs.h"
#include "Signal.hpp"

using std::cout, std::endl;
/*****************
 * Signal handlers
 *****************/

/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
// void sigchld_handler(int sig) 
//  {
//    int old_errno = errno;
//    pid_t pid;
//    int status;
//    sigset_t mask_all, prev_all; 
     
//    sigfillset(&mask_all);  
 
//    while((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0)
//    {
//      BLOCK(mask_all, prev_all);
//      struct job_t *job = getjobpid(jobs, pid);
 
//      if (pid == fgpid(jobs)){
//        FG_PID_GLOBALS = pid;
//      }
 
//      if (WIFEXITED(status))  // Normal: delete job
//      {
//        deletejob(jobs, pid);
//      }
//      else if (WIFSIGNALED(status))  // C^C: print,and delete job
//      {
//        printf("Job [%d] (%d) terminated by signal %d\n", job->jid, job->pid, WTERMSIG(status));
//        deletejob(jobs, pid);
//      }
//      else if (WIFSTOPPED(status)) // C^Z: print,and moditfy job state.
//      {
//        printf("Job [%d] (%d) stopped by signal %d\n", job->jid, job->pid, WSTOPSIG(status));
//        job->state = ST;
//      }
//      else{
//        deletejob(jobs, pid);
//        printf("hahaha\n");
//      }
 
//      UNBLOCK(prev_all);
//    }
 
//    errno = old_errno;
//  }
/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig) {
    cout << "sigint_handler...." << endl;
  //  int old_errno = errno;
   
  //  pid_t pid = fgpid(jobs);
  //  if (pid != 0)
  //    kill(-pid, sig);
     
  //  errno = old_errno;
 }

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) {
    cout << "sigtstp_handler...." << endl;
  //  int old_errno = errno;
  //  pid_t pid = fgpid(jobs);
 
  //  if (pid != 0)
  //    kill(-pid, sig);
 
  //  errno = old_errno;
 }


/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) 
{
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}

/*********************
 * End signal handlers
 *********************/

