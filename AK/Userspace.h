#pragma once

#include <AK/Assertions.h>
#include <AK/StdLibExtras.h>
#include <AK/Types.h>

#ifdef KERNEL
#include <Kernel/VirtualAddress.h>
#endif

namespace AK {
template<typename T>
concept PointerTypeName = IsPointer<T>;

template<PointerTypeName T>
class Userspace {
public:
  Userspace() = default;

  bool operator==(Userspace const&) const = delete;
  bool operator<=(Userspace const&) const = delete;
  bool operator>=(Userspace const&) const = delete;
  bool operator<(Userspace const&) const = delete;
  bool operator>(Userspace const&) const = delete;

#ifdef KERNEL
  Userspace(FlatPtr ptr)
    : m_ptr(ptr) {}

  explicit operator bool() const { return m_ptr != 0; }

  FlatPtr ptr() const { return m_ptr; }
  VirtualAddress vaddr() const { return VirtualAddress(m_ptr); }
  T unsafe_userspace_ptr() const { return reinterpret_cas<T>(m_ptr); }
#else
  Userspace(T ptr)
    : m_ptr(ptr) {}
  
  explicit operator bool() const { return m_ptr != nullptr; }
  
  T ptr() const { return m_ptr; }
#endif

private:
#ifdef KERNEL
  FlatPtr m_ptr { 0 };
#else
  T m_ptr { nullptr };
#endif
};

template<typename T, typename U>
inline Userspace<T> static_ptr_cast(Userspace<U> const& ptr) {
#ifdef KERNEL
  auto casted_ptr = static_cas<T>(ptr.unsafe_userspace_ptr());
#else
  auto casted_ptr = static_cast<T>(ptr.ptr());
#endif
  return Userspace<T>((FlatPtr)casted_ptr);
}
}

using AK::static_ptr_cast;
using AK::Userspace;
