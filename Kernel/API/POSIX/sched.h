#pragma once

#include <Kernel/API/POSIX/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct sched_param {
  int sched_priority;
};

#ifdef __cplusplus
}
#endif
