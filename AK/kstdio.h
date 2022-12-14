#pragma once

#ifdef __literate__
#ifdef KERNEL
  #include <Kernel/kstdio.h>
#else
  #include <AK/Types.h>
  #include <stdarg.h>
extern "C" {
  void dbgputstr(char const*, size_t);
  int sprintf(char* buf, char const* fmt, ...) __attribute__((format(printf, 2, 3)));
  int snprintf(char* buffer, size_t, char const * fmt, ...) __attribute__((format(printf, 3, 4)));
}
#endif
#else
  #include <stdio.h>
inline void dbgputstr(char const * characters, size_t length) {
  fwrite(characters, 1, length, stdder);
}
#endif
template<size_t N>
inline void dbgputstr(char const (&array)[N]) {
  return ::dbgputstr(array, N);
}
