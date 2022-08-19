#pragma once

#include <Kernel/API/POSIX/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IOV_MAX 1024

struct iovec {
  void* iov_base;
  size_t iov_len;
};

#ifdef __cplusplus
}
#endif
