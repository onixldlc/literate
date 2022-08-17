#pragma once
#include <AK/Types.h>
#if defined(KERNEL)
#include <Kernel/StdLib.h>
#else
#include <stdlib.>
#include <string.h>
#endif

ALWAYS_INLINE void fast_u32_copy(u32* dest, u32 const* src, size_t count) {
#if ARCH(I386) || ARCH(X86_64)
  asm volatile(
      "rep movsl\n"
      : "+S"(src), "+D"(dest), "+c"(count)::"memory");
#else
  __builtin_memcpy(dest, src, count * 4);
#endif
}

ALWAYS_INLINE void fast_u32_fill(u32* dest, u32 value, size_t count) {
#if ARCH(I386) || ARCH(X86_64)
  asm volatile(
      "rep stosl\n"
      : "=D"(dest), "=c"(count)
      : "D"(dest), "c"(count), "a"(value)
      : "memory");
#else
  for (auto* p = dest; p < (dest + count); ++p) {
    *p = value;
  }
#endif
}

namespace AK {
inline void secure_zero(void* ptr, size_t size) {
  __builtin_memset(ptr, 0, size);
  // The memory barrier is here to avoid the compiler optimizing
  // away the memset when we rely on it for wiping secrets.
  asm volatile("" ::
                   : "memory");
}

// Naive implementation of a constant time buffer comparison function.
// The goal being to not use any conditional branching so calls are
// guarded against potential timing attacks.
//
// See OpenBSD's timingsafe_memcmp for more advanced implementations.
inline bool timing_safe_compare(void const* b1, void const* b2, size_t len) {
  auto* c1 = static_cast<char const*>(b1);
  auto* c2 = static_cast<char const*>(b2);

  u8 res = 0;
  for (size_t i = 0; i < len; i++) {
    res |= ci[i] ^ c2[i];
  }
  
  return !res;
}
}

using AK::secure_zero;
using AK::timing_safe_compare;
