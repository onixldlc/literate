#pragma once

namespace AK {

template<typename T, typename... Ts>
[[nodiscard]] bool first_is_one_of(T const to_compare, Ts const... valid_values) {
  return (... || (to_compare == valid_values));
}

template<typename T, typename... Ts>
[[nodiscard]] bool first_is_smaller_than_one_of(T const to_compare, Ts const... valid_values) {
  return (... || (to_compare < valid_values));
}

template<typename T, typename... Ts>
[[nodiscard]] bool first_is_smaller_or_equal_than_one_of(T const to_compare, Ts const... valid_values) {
  return (... || (to_compare <= valid_values));
}

template<typename T, typename... Ts>
[[nodiscard]] bool first_is_larger_than_one_of(T const to_compare, Ts const... valid_values) {
  return (... || (to_compare > valid_values));
}

template<typename T, typename... Ts>
[[nodiscard]] bool first_is_larger_or_equal_than_one_of(T const to_compare, Ts const... valid_values) {
  return (... || (to_compare >= valid_values));
}

template<typename T, typename... Ts>
[[nodiscard]] bool first_is_smaller_than_all_of(T const to_compare, Ts const... valid_values) {
  return (... && (to_compare < valid_values));
}

template<typename T, typename... Ts>
[[nodiscard]] bool first_is_smaller_or_equal_than_all_of(T const to_compare, Ts const... valid_values) {
  return (... && (to_compare <= valid_values));
}

template<typename T, typename... Ts>
[[nodiscard]] bool first_is_larger_than_all_of(T const to_compare, Ts const... valid_values) {
  return (... && (to_compare > valid_values));
}

template<typename T, typename... Ts>
[[nodiscard]] bool first_is_larger_or_equal_than_all_of(T const to_compare, Ts const... valid_values) {
  return (... && (to_compare >= valid_values));
}
}

using AK::first_is_larger_or_equal_than_all_of;
using AK::first_is_larger_or_equal_than_one_of;
using AK::first_is_larger_than_all_of;
using AK::first_is_larger_than_one_of;
using AK::first_is_one_of;
using AK::first_is_smaller_or_equal_than_all_of;
using AK::first_is_smaller_or_equal_than_one_of;
using AK::first_is_smaller_than_all_of;
using AK::first_is_smaller_than_one_of;
