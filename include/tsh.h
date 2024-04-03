#pragma once  

/* 
 * tsh - A tiny shell program with job control
 */

#include "jobs_types.h"
#include <vector>


extern bool verbose; 


void eval(const std::string &command_line);  


/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */
bool isbuiltinCommand(const std::vector<const char *> argv);


/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
JobState parseline(const std::string &cmdline, std::vector<const char *> &argv); 

// in "lib/tshlib.h"
void usage(void);
