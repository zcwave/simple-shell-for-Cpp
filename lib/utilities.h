#pragma once


#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>


/*
 * usage - print a help message
 */
void usage(void);


/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg);


/*
 * app_error - application-style error routine
 */
void app_error(char *msg);