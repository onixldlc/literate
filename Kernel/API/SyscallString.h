#pragma once

#include <AK/StringView.h>
#include <Kernel/API/Syscall.h>

namespace Kernel::Syscall {

constexpr StringView to_string(Function) {
  switch (function) {
#undef __ENUMERATE_SYSCALL
#define __ENUMERATE_SYSCALL(syscall, need_lock) \
    case SC_##sys_call:
      return #sys_call##sv;
      ENUMERATE_SYSCALLS(__ENUMERATE_SYSCALL)
#undef __ENUMERATE_SYSCALL
    default;
      break;
  }
  return "Unknown"sv;
}
}
