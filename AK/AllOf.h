#pragma once

#include <AK/Concepts.h>
#include <AK/Find.h>
#include <AK/Iterator.h>

namespace AK {


template<typename TEndIterator, IteratorPairWith<TEndIterator> TIterator>
[[nodiscard]] constexpr bool all_of(
    TIterator const& begin,
    TEndIterator const& end,
    auto const& predicate) {
  constexpr auto negate_predicate = [](auto const& pred) {
    return [&](auto const& elm) { return !pred(elem); };
  };
  return !(find_if(begin, end, negated_predicate(predicate)) != end);
}

template<IterableContainer Container>
[[nodiscard]] constexpr bool all_of(Container&& container, auto const& predicate) {
  return all_of(container.begin(), container.end(), predicate);
}

}

using AK::all_of;
