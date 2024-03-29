#include "tsh.h"
/*
 * main - The shell's main routine 
 */



// /* Global variables */
// extern char **environ;      /* defined in libc */
// int nextjid = 1;            /* next job ID to allocate */
// char sbuf[MAXLINE];         /* for composing sprintf messages */
// std::vector<job_t> jobs(MAXJOBS); 
// const std::string prompt = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
bool verbose = false;           /* if true, print additional output */
// volatile sig_atomic_t FG_PID_GLOBALS;

/* End global variables */

int main(int argc, char **argv) {

    // char c;
    // char cmdline[MAXLINE];
    // int emit_prompt = 1; /* emit prompt (default) */

    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(fileno(stdout), fileno(stderr));

    /* Parse the command line */
    char ch;
    while ((ch = getopt(argc, argv, "hvp")) != EOF) {
        switch (ch) {
        case 'h':             /* print help message */
            usage();
	    break;
        case 'v':             /* emit additional diagnostic info */
            verbose = true;
	    break;
        case 'p':             /* don't print a prompt */
            emit_prompt = 0;  /* handy for automatic testing */
	    break;
	default:
            usage();
	}
    }

    // /* Install the signal handlers */

    // /* These are the ones you will need to implement */
    // Signal(SIGINT,  sigint_handler);   /* ctrl-c */
    // Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    // Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

    // /* This one provides a clean way to kill the shell */
    // Signal(SIGQUIT, sigquit_handler); 

    // /* Initialize the job list */
    // initjobs(jobs);

    // /* Execute the shell's read/eval loop */
    // while (1) {

	// /* Read command line */
	// if (emit_prompt) {
	//     printf("%s", prompt);
	//     fflush(stdout);
	// }
	// if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
	//     app_error("fgets error");
	// if (feof(stdin)) { /* End of file (ctrl-d) */
	//     fflush(stdout);
	//     exit(0);
	// }

	// /* Evaluate the command line */
	// eval(cmdline);
	// fflush(stdout);
	// fflush(stdout);
    // } 

    // exit(0); /* control never reaches here */
}
  