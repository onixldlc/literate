#pragma once

#ifndef KERNEL
#include <AK/String.h>
#include <AK/StringView.h>
#include <cxxabi.h>

namespace AK {

inline String demangle(StringView name) {
  int status = 0;
  auto* demangled_name = abi::__cxa_demangle(name.to_string().characters(), nullptr, nullptr, &status);
  auto string = String(status == 0 ? StringView { demangled_name, strlen(demangled_name) } : name);
  if (status == 0)
    free(demangled_name);
  return string;
}

}

using AK::demangle;

#endif
