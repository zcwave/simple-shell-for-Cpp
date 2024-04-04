#pragma once

/* 
 * tsh - A tiny shell program with job control
 */

#include <signal.h>
#include <vector>
#include "jobs_types.h"

constexpr int MAXJOBS = 16;       /* max jobs at any point in time */
constexpr int MAXJID  = 1 << 16;  /* max job ID : 65536 */ 

extern bool verbose; 
extern volatile sig_atomic_t FG_PID_GLOBALS;


void eval(const std::string &command_line);  
bool isbuiltinCommand(std::vector<const char *> &argv);
JobState parseline(const std::string &cmdline, std::vector<const char *> &argv); 

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(std::vector<const char *> &argv);
void waitfg(pid_t pid);

// in 'lib/utilities.h
void usage(void);

