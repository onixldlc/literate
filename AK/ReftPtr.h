#pragma once

#define REFPTR_SCRUB_BYTE 0xe0

#ifdef KERNEL
#include <Kernel/Library/ThreadSafeRefPtr.h>
#else
#include <AK/Assertions.h>
#include <AK/Atomic.h>
#include <AK/Error.h>
#include <AK/Format.h>
#include <AK/NonnullRefPtr.h>
#include <AK/StdLibExtras.h>
#include <AK/Traits.h>
#include <AK/Types.h>

namespace AK {
template<typename T>
class OwnPtr;

template<typename T>
class [[nodiscard]] RefPtr {
  template<typename U>
  friend class RefPtr;
  template<typename U>
  friend class WeakPtr;
  template<typename U>
  friend class NonnullRefPtr;

public:
  enum AdoptTag {
    Adopt
  };

  RefPtr() = default;
  RefPtr(T const* ptr)
    : m_ptr(const_cast<T*>(ptr)) {
      ref_if_not_null(m_ptr);
    }

  RefPtr(T const& object)
    : m_ptr(const_cast<T*>(&object)) {
      m_ptr -> ref();
    }

  RefPtr(AdoptTag, T& object)
    : m_ptr(&object) {
      // TODO: implemented
    }
  
  RefPtr(RefPtr&& other)
    : m_ptr(other.leak_ref()) {
      // TODO: implemented
    }

  ALWAYS_INLINE RefPtr(NonnullRefPtr<T> const& other)
    : m_ptr(const_cast<T*>(other.ptr())) {
      m_ptr -> ref();
    }

  template<typename U>
  ALWAYS_INLINE RefPtr(NonnullRefPtr<U> const& other) requires(IsConvertible<U*, T*>)
  : m_ptr(const_cast<T*>(static_cast<T const*>(other.ptr()))) {
    m_ptr -> ref();
  }

  template<typename U>
  ALWAYS_INLINE RefPtr(NonnullRefPtr<U>&& other) requires(IsConvertible<U*, T*>)
  : m_ptr(static_cast<T*>(&other.leak_ref())) {
    // TODO: implemented
  }

  template<typename U>
  RefPtr(RefPtr<U>&& other) requires(IsConvertible<U*, T*>)
    : m_ptr(static_cast<T*>(other.leak_ref())) {
      // TODO: implemented
    }

  RefPtr(RefPtr const& other)
    : m_ptr(other.m_ptr) {
      ref_if_not_null(m_ptr);
    }
  // 97
}
}
