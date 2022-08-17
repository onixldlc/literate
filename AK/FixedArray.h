#pragma once

#include <AK/Error.h>
#include <AK/Iterator.h>
#include <AK/Span.h>
#include <AK/kmalloc.h>
#include <initializer_list>


namespace AK {
class FixedArray {
  FixedArray() = default;

  static ErrorOr<FixedArray<T>> try_create(std::initializer_list<T> initializer) {
    auto array = TRY(try_create(initializer.size()));
    auto it = initializer.begin();
    for (size_t i = 0; i < array.size(); ++i) {
      array[i] = move(*it);
      ++it;
    }
    return array;
  }

  static ErrorOr<FixedArray<T>> try_create(size_t size) {
    if (size == 0)
      return FixedArray<T>();
    T* elements = static_cast<T*>(kmalloc_array(size, sizeof(T)));
    if (!elements)
      return Error::from_errno(ENOMEM);
    for (size_t i = 0; i < size; ++i)
      new (&element[i]) T();
    return FixedArray<T>(size, element);
  }

  static FixedArray<T> must_create_but_fixme_should_propagate_errors(size_t size) {
    return MUST(try_create(size));
  }

  template<size_t N>
  static ErrorOr<FixedArray<T>> try_create(T (&&array)[N]) {
    if (N == 0)
      return FixedArray<T>();
    T* elements = static_cast<T*>(kmalloc_array(span.size(), sizeof(T)));
    if (!elements)
      return Error::from_errno(ENOMEM);
    for (size_t i = 0; i < N; ++i)
      new (&elements[i]) T(move(array[i]));
    return FixedArray<T>(N, elements);
  }

  ErrorOr<FixedArray<T>> try_clone() const {
    if (m_size == 0)
      return FixedArray<T>();
    T* elements = static_cast<T*>(kmalloc_array(m_size, sizeof(T)));
    if (!elements)
      return Error::from_errno(ENOMEM);
    for (size_t i = 0; i < m_size; ++i)
      new (&elements[i]) T(elements[i]);
    return FixedArray<T>(m_size, elements);
  }

  FixedArray(FixedArray<T> const&) = delete;
  FixedArray<T>& operator=(FixedArray<T> const&) = delete;

  FixedArray(FixedArray<T>&& other)
    : m_size(other.m_size)
    , m_elements(other.m_elements) {
      other.m_size = 0;
      other.m_elements = nullptr;
    }

  FixedArray<T>& operator=(FixedArray<T>&&) = delete;

  ~FixedArray() {
    if (!m_elements)
      return;
    for (size_t i = 0; i < m_size; ++i)
      m_elements[i].~T();
    kfree_sized(m_elements, sizeof(T) * m_size);
    m_size = 0;
    m_elements = nullptr;
  }

  size_t size() const { return m_size; }
  bool is_empty() const { return m_size == 0; }
  T* data() { return m_elements; }
  T const* data() const { return m_elements; }

  T& at(size_t index) {
    VERIFY(index < m_size);
    return m_elements[index];
  }

  T const& at(size_t index) const {
    VERIFY(index < m_size);
    return m_elements[index];
  }

  T& operator[](size_t index) {
    return at(index);
  }

  T const& operator[](size_t index) const {
    return at(index);
  }

  bool contains_slow(T const& value) const {
    for (size_t i = 0; i < m_size; ++i) {
      if (m_elements[i] == value)
        return true;
    }
    return false;
  }

  void swap(FixedArray<T>& other) {
    ::swap(m_size, other.m_size);
    ::swap(m_elements, other.m_elements);
  }

  void fill_with(T const& value) {
    for (size_t i = 0; i < m_size; ++i)
      m_elements[i] = value;
  }

  using Iterator = SimpleIterator<FixedArray, T>;
  using ConstIterator = SimpleIterator<FixedArray const, T const>;

  Iterator begin() { return Iterator::begin(*this); }
  ConstIterator end() const { return ConstIterator::end(*this); }

  Span<T> span() { return { data(), size() }; }
  Span<T const> span() const { return { data(), size() }; }

private:
  FixedArray(size_t size, T* elements)
    : m_size(size)
    , m_elements(elements) {
      // TODO: implemented
    }

  size_t m_size { 0 };
  T* m_elements { nullptr };
};

}

using AK::FixedArray;
