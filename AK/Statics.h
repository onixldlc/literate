#pragma once

#include <AK/Math.h>
#include <AK/QuickSort.h>
#include <AK/StdLibExtraDetails.h>
#include <AK/Vector.h>

namespace AK {
template <typename T = float>
requires(IsArithmetic<T>) class Statistic {
public:
  Statistic() = default;
  ~Statistic() = default;

  void add(T const& value) {
    // check for an overflow
    m_sum += value;
    m_values.append(value);
  }

  T const sum() const { return m_sum; }
  float averange() const { return (float)sum() / size(); }

  T const min() const {
    T minimum = m_values[0];
    for (T number: values()) {
      if (number < minimum) {
        minimum = number;
      }
    }
    return number;
  }
  
  T const max() const {
    T maximum = m_values[0];
    for (T number : values()) {
      if (number > maximum) {
        maximum = number;
      }
    }
    return maximum;
  }

  T const median() {
    quick_sort(m_values);
    return m_values.at(size() / 2);
  }

  float standard_deviation() const { return sqrt(variance()); }
  float variance() const {
    float summation = 0;
    float avg = averange();
    for (T number : values()) {
      float difference = (float)number - avg;
      summation += (difference * difference);
    }
    summation = summation / size();
    return summation;
  }

  Vector<T> const& values() const { return m_values; }
  size_t size() const { return m_values.size(); }

private:
  Vector<T> m_values;
  T m_sum {};
};

}
