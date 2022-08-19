#pragma once

#include <AK/Types.h>

namespace Kernel::RPi {

class MMIO {
public:
  static MMIO& the();

  u32 read(FlatPtr offset) { return *peripheral_address(offset); }
    void write(FlatPtr offset, u32 value) { *peripheral_address(offset) = value; }

  u32 volatile* peripheral_address(FlatPtr offset) { return (u32 volatile*)(m_base_address + offset); }
  template<class T>
  T volatile* peripheral(FlatPtr offset) { return (T volatile*)peripheral_address(offset); }

  FlatPtr peripheral_base_address() const { return m_base_address; }
  FlatPtr peripheral_end_address() const { return m_base_address + 0x00FFFFFF; }

private:
  MMIO();

  unsigned int m_base_address;
};

}
