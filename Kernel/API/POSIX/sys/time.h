#pragma once

#include <Kernel/API/POSIX/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct timeval {
  time_t tv_sec;
  subseconds_t tv_usec;
};

#ifdef __cplusplus
}
#endif
