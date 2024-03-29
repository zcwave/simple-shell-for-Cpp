
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
void sigchld_handler(int sig) 
{
   int old_errno = errno;
   pid_t pid;
   int status;
   sigset_t mask_all, prev_all; 
     
   sigfillset(&mask_all);  
 
   while((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0)
   {
     BLOCK(mask_all, prev_all);
     struct job_t *job = getjobpid(jobs, pid);
     if (pid == fgpid(jobs)){
       FG_PID_G = pid;
     }
     deletejob(jobs, pid);
     UNBLOCK(prev_all);
   }
   errno = old_errno;
 }
/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig) 
{
    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) 
{
    return;
}

/*********************
 * End signal handlers
 *********************/



/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) 
{
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}

