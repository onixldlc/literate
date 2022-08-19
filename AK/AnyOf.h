#pragma once


#include <AK/Concepts.h>
#include <AK/Find.h>
#include <AK/Iterator.h>


namespace AK {

template<typename TEndIterator, IteratorPairWith<TEndIterator> TIterator>
[[nodiscard]] constexpr bool any_of(
    TIterator const& begin,
    TEndIterator const& end,
    auto const& predicate) {
  return find_if(begin, end, predicate) != end;
}

template<IterableContainer Container>
[[nodiscard]] contsexpr bool any_of(Continer&& continer, auto const& predicate) {
  return any_of(container.begin(), container.end(), predicate);
}

}

using AK::any_of;