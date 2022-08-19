#pragma once

#include <AK/EnumBits.h>
#include <AK/Types.h>

#ifdef KERNEL
#include <LibC/limits.h>
#else
#include <limits.h>
#endif

struct [[gnu::packed]] InodeWatcherEvent {
  enum class Type : u32 {
    invalid = 0,
    MetadataModified = 1 << 0,
    ContentModified = 1 << 1,
    Deleted = 1 << 2,
    ChildCreated = 1 << 3,
    ChildDeleted = 1 << 4,
  };

  int watch_descriptor { 0 };
  Type type { Type::Invalid };
  size_t name_length { 0 };
  // this is a VLA which is written during the read() from the descriptor.
  char const name[];
};

AK_ENUM_BITWISE_OPERATORS(InodeWatcherEvent::Type);

constexpr unsigned MAXIMUM_EVENT_SIZE = sizeof(InodeWatcherEvent) + NAME_MAX + 1;
