#include "tsh.h"
#include "Signal.hpp"
#include "Jobs.hpp"
// #include "userlib.h"
/*
 * main - The shell's main routine 
 */



// /* Global variables */
extern char **environ;      /* defined in libc */
// int nextjid = 1;            /* next job ID to allocate */
// char sbuf[MAXLINE];         /* for composing sprintf messages */
// std::vector<job_t> jobs(MAXJOBS); 
const std::string prompt {"tsh> "};    /* command line prompt (DO NOT CHANGE) */
bool verbose { false };           /* if true, print additional output */
volatile sig_atomic_t FG_PID_GLOBALS;

/* End global variables */

int main(int argc, char **argv) {

    bool emit_prompt = true; /* emit prompt (default) */

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
            emit_prompt = false;  /* handy for automatic testing */
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
    Signal(SIGQUIT, sigquit_handler); 

    // /* Initialize the job list */
    // initjobs(jobs);

    
    std::string command{};
    using std::cin;
    using std::flush;
    using std::cout;
    /* Execute the shell's read/eval loop */
    while (true) {
        /* Read command line */
        if (emit_prompt) {
            cout << prompt << flush;
        }
        if (!std::getline(cin, command)) {
            if (cin.eof()) { /* End of file (ctrl-d) */
                cout << flush;
                exit(0);
            }
            else throw std::runtime_error("read error");
        }

        /* Evaluate the command line */
        eval(command); 
        cout << flush;
        cout << flush;
    } 

    exit(0); /* control never reaches here */
}
