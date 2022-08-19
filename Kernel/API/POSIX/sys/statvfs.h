#pragma once

#include <Kernel/API/POSIX/sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ST_RDONLY 0x1
#define ST_NOSUID 0x2

#define FSTYPSZ 16

struct statvfs {
  unsigned long f_bsize;
  unsigned long f_frsize;
  fsblkcnt_t f_blocks;
  fsblkcnt_t f_bfree;
  fsblkcnt_t f_bavail;

  fsfilcnt_t f_files;
  fsfilcnt_t f_ffree;
  fsfilcnt_t f_favail;

  unsigned long f_fsid;
  unsigned long f_flag;
  unsigned long f_namemax;

  char f_basetype[FSTYPSZ];
};

#ifdef __cplusplus
}
#endif
