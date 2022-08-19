#pragma once

#include <Kernel/API/sys/socket.h>
#include <Kernel/API/POSIX/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct rtentry {
  struct sockaddr rt_dst;
  struct sockaddr rt_gateway;
  struct sockaddr rt_genmask;
  unsigned short int rt_flags;
  char* rt_dev;
};

#define RTF_UP 0x1
#define RTF_GATEWAY 0x2
#define RTF_HOST 0x4

#ifdef __cplusplus
}

#endif
