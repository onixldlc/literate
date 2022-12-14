#pragma once

namespace AK {
class Badge {
public:
  using Type = T;

private:
  friend T;
  constexpr Badge() = default;

  Badge(Badge const&) = delete;
  Badge& operator=(Badge const&) = delete;

  Badge(Badge&&) = delete;
  Badge& operator=(&Badge) = delete;
};
}

using AK::Badge;
