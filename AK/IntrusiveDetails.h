#pragma once

#include <AK/NonnullRefPtr.h>

namespace AK::Detail {

template<typename T, typename Container>
struct SubstituteIntrusiveContainerType {
    using Type = Container;
};

template<typename T>
struct SubstituteIntrusiveContainerType<T, NonnullRefPtr<T>> {
    using Type = RefPtr<T>;
};

template<typename Container, bool _IsRaw>
struct SelfReferenceIfNeeded {
    Container reference = nullptr;
};
template<typename Container>
struct SelfReferenceIfNeeded<Container, true> {
};

}
