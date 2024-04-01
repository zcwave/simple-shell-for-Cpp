#pragma once


#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);