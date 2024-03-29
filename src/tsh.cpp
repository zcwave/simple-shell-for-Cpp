#include "tsh.h"
/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
*/
void eval(char *cmdline) 
 {
   char *argv[MAXARGS]; // argument list exec()
   char buf[MAXLINE]; // Holds modified commmand line.
   int bg; // should the job run in bg or fg?
   pid_t pid;
 
   strcpy(buf, cmdline);
   bg = parseline(buf, argv); 
   // constructing argv[MAXARGS] And return true if job is bg. iow, the last char is &.
 
   if (argv[0] == NULL)
     return; // Ignore empty lines.
 
 
   if (!builtin_cmd(argv)){
 
     sigset_t mask_all;
     sigset_t mask_one, prev_one;
 
     Sigfillset(&mask_all);
     Sigemptyset(&mask_one);
     Sigaddset(&mask_one, SIGCHLD);
     
     BLOCK(mask_one, prev_one); // Block SIGCHLD.
     
     pid = Fork();
 
     if (pid == 0){
       // child runs user job.
       UNBLOCK(prev_one); //Unblock SIGCHLD.
       setpgid(0, 0);
       if (execve(argv[0], argv, environ) < 0){
         printf("%s: Command not found. \n", argv[0]);
         exit(0);
       }
     }
     else {
       int state = bg ? BG : FG;
       BLOCK_NOT_SAVE_OLD_SET(mask_all);
       addjob(jobs, pid, state, cmdline);
       UNBLOCK(prev_one); // Unblock SIGCHLD.
 
       /* Parent waits for fg job to terminate.*/
       if (!bg){
         waitfg(pid);
       }
       else {
         // the job is bg.
         BLOCK_NOT_SAVE_OLD_SET(mask_all);
         printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);
       }  
       UNBLOCK(prev_one);//Unblock all signal
     } 
   }
 }

/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
int parseline(const char *cmdline, char **argv) 
{
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
	buf++;
	delim = strchr(buf, '\'');
    }
    else {
	delim = strchr(buf, ' ');
    }

    while (delim) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* ignore spaces */
	       buf++;

	if (*buf == '\'') {
	    buf++;
	    delim = strchr(buf, '\'');
	}
	else {
	    delim = strchr(buf, ' ');
	}
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* ignore blank line */
	return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) {
	argv[--argc] = NULL;
    }
    return bg;
}

/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */
int builtin_cmd(char **argv) 
 {
 
   if (!strcmp(argv[0], "quit"))// quit command.
     exit(0); 
   else if (!strcmp(argv[0], "&")) // Ignore singleton &. nothing is happen.
     return 1; 
   else if (!strcmp(argv[0], "jobs")){// jobs command.
     sigset_t mask_all, prev_all;
     Sigfillset(&mask_all);
 
     BLOCK(mask_all, prev_all);
     listjobs(jobs);
     UNBLOCK(prev_all);
     
     return 1; 
   }
  else if (!strcmp(argv[0], "bg") || !strcmp(argv[0], "fg")){ // bg or fg commands.
   do_bgfg(argv);
   return 1;
 }
   return 0;     /* not a builtin command */
 }

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) 
 {
   struct job_t *job;
   int id;
   sigset_t mask_all, prev_all;
   Sigfillset(&mask_all);
 
   if (argv[1] == NULL){
     // single fg or bg.
     printf("%s command requires PID or %%jobid argument\n", argv[0]);
     return;
   }
   else if (sscanf(argv[1], "%%%d", &id) > 0){
     BLOCK(mask_all, prev_all);
     job = getjobjid(jobs, id);
     UNBLOCK(prev_all);
 
     if (job == NULL){
       // not found the job.
       printf("%%%d: No such job\n", id);
       return;
     }
   }
   else if(sscanf(argv[1], "%d", &id) > 0){
     BLOCK(mask_all, prev_all);
     job = getjobpid(jobs, id);
     UNBLOCK(prev_all); 
 
     if (job == NULL){
       printf("(%d): No such process\n", id);
       return;
     }
   }
   else{
     // not pid or jid.
     printf("%s: argument must be a PID or %%jobid\n", argv[0]);
     return;
   }
 
   if (!strcmp(argv[0], "bg")){
     kill(-(job->pid), SIGCONT);
     job->state = BG;
     printf("[%d] (%d) %s", job->jid, job->pid, job->cmdline);
   }
   else if (!strcmp(argv[0], "fg")){
     kill(-(job->pid), SIGCONT);
     job->state = FG;
     waitfg(job->pid);
   }
 }

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
 {
   sigset_t mask;
   sigemptyset(&mask);
 
   FG_PID_GLOBALS = 0;
   while (!FG_PID_GLOBALS)
     sigsuspend(&mask);
 }






