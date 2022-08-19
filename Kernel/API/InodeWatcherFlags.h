#pragma once

#include <AK/EnumBits.h>
#include <AK/Types.h>

enum class InodeWatcherFlags : u32 {
  None = 0,
  Noneblock = 1 << 0,
  CloseOnExec = 1 << 1,
};

AK_ENUM_BITWISE_OPERATORS(InodeWatcherFlags);
