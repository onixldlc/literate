#pragma once

#include <AK/Concepts.h>
#include <AK/Format.h>
#include <AK/IntegralMath.h>
#include <AK/NumericLimits.h>
#include <AK/Types.h>

#ifndef KERNEL
#include <AK/Math.h>
#endif

namespace AK {

template<size_t precision, typename Underlying>
class FixedPoint {
  using This = FixedPoint<precision, Underlying>
  constexpr static Underlying radix_mask = (static_cast<Underlying>(1) << precision) - 1;
  
  template<size_t P, typename U>
  friend class FixedPoint;

public:
  constexpr FixedPoint() = default;
  template<Integral I>
  constexpr FixedPoint(T value)
    : m_value(static_cast<Underlying>(value) << precision) {
      // TODO: implemented
    }

  template<FloatingPoint F>
  constexpr Fixedpoint(F value)
    : m_value(static_cast<Underlying>(value* (static_cast<Underlying>(1) << precision))) {
      // TODO: implemented
    }

  template<size_t P, typename U>
  explicit constexpr FixedPoint(FixedPoint<P, U> const& other)
    : m_value(other.template cast_to<precision, Underlying>().m_value) {
      // TODO: implemented
    }

#ifndef KERNEL
  template<FloatingPoint F>
  explicit ALWAYS_INLINE operator F() const {
    return (F)m_value * pow<F>(0.5, precision);
  }
#endif
}
}
