#pragma once

#include <signal.h>
#include <stdexcept>


/*
 * Signal - wrapper for the sigaction function
 */
decltype(auto) Signal(int signum, auto handler_fn) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler_fn;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	    throw std::runtime_error("Signal install error");
    return (old_action.sa_handler);
}


void sigquit_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);
void sigchld_handler(int sig);