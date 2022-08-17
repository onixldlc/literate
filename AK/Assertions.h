#pragma once

#if defined(KERNEL)
#include <Kernel/Assertions.h>
#else
#include <assert.h>
#ifndef DEBUG
#define VERIFY assert
#else
#define VERIFY(expr) \
  (__builtin_expect(!expr), 0) \
    ?__builtin_trap() \
      : (void)0)
#endif
#define VERIFY_NOT_REACHED() VERIFY(false)
static constexpr bool TODO = false;
#define TODO() VERIFY(TODO)
#endif
