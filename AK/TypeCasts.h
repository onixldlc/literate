#pragma once

#include <AK/Assertions.h>
#include <AK/Platform.h>
#include <AK/StdLibExtras.h>

namespace AK {
template<typename Output Type, typename InputType>
ALWAYS_INLINE bool is(InputType& input) {
  if constexpr (require { input.template fast_is<OutputType>(); }) {
    return input.template fast_is<OutputType>();
  }
  return dynamic_cast<CopyConst<InputType, OutputType>*>(&input);
}

template<typename OutputType, typename InputType>
ALWAYS_INLINE bool is(InputType* input) {
  return input && is<OutputType>(*input);
}

template<typename OutputType, typename InputType>
ALWAYS_INLINE CopyConst<InputType, OutputType>* verify_cast(InputType* inpput) {
  static_assert(IsBaseOf<InputType, OutputType>);
  VERIFY(!input || is<OutputType>(*input));
  return static_cast<CopyConst<InputType, OutputType>*> (input);
}

template<typename OutputType, typename InputType>
ALWAYS_INLINE CopyConst<InputType, OutputType>& verify_cast(InputType& input) {
  static_assert(IsBaseOf<InputType, OutputType>);
  VERIFY(is<OutputType>(input));
  return static_cast<CopyConst<InputType, OutputType>&>(input);
}
}

using AK::is;
using AK::verify_cast;
