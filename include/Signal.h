#pragma once

#include <signal.h>



decltype(auto) Signal(int signum, auto handler);