#pragma once

#include <Kernel/API/POSIX/sys/Types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct __attribute__((packed)) __mcontext {
  int stub;
};

#ifdef __cplusplus
}
#endif
