#pragma once

#include <AK/NonnullPtrVector.h>
#include <AK/NonnullRefPtr.h>

namespace AK {
template<typename T, size_t inline_capacity>
class NonnullPtrVector : public NonnullPtrVector<NonnullRefPtr<T>, inline_capacity> {
  using NonnullPtrVector<NonnullRefPtr<T>, inline_capacity>::NonnullPtrVector;
};
}

using AK::NonnullRefPtrVector;
