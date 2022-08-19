#pragma once

#include <AK/Types.h>

#ifdef KERNEL
#include <Kernel/UnixTypes.h>
#else
#include <time.h>
#endif

namespace Kernel {
inline bool time_page_supports(clockid_t clock_id) {
  return clock_id == CLOCK_REALTIME_COARSE || clock_id == CLOCK_MONOTONIC_COARSE;
}

struct TimePage {
  volatile u32 update1;
  struct timespec clocks[CLOCK_ID_COUNT];
  volatile u32 update2;
};

}
