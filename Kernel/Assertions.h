#pragma once

#define __STRINGIFY_HELPER(X) #x
#define __STRINGIFY(x) __STRINGIFY_HELPER(x)

extern "C" {
  [[noreturn]] void _abort();
  [[noreturn]] void abort();
}

#define TODO() __assertion_failed("TODO", __FILE__, __LINE__, __PRETTY_FUNCTION__)

//  i386 0x200 !cpuflags disable
// x86_64 0x200 cpu_flags enable
