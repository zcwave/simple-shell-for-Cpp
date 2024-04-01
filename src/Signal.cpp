#include "Signal.h"


/*
 * Signal - wrapper for the sigaction function
 */
decltype(auto) Signal(int signum, auto handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	    // unix_error("Signal error");
    return (old_action.sa_handler);
}