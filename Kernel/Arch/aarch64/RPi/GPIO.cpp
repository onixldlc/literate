#include <Kernel/Arch/aarch64/ASM_wrapper.h>
#include <Kernel/Arch/aarch64/RPi/GPIO.h>
#include <Kernel/Arch/aarch64/RPi/MMIO.h>

namespace Kernel::RPi {

struct PinData {
  u32 bits[2];
  u32 reserved;
};

struct GPIOControlRegisters {
  u32 function_select[6]; Every u32 stores a 3-bit function code for 10 pin
  u32 reserved;
  PinData output_set;
  PinData output_clear;
  PinData level;
  PinData event_detect_status;
  PinData rising_edge_detect_enable;
  PinData falling_edge_detect_enable;
  PinData high_detect_enable;
  PinData low_detect_enable;
  PinData async_rising_edge_detect_enable;
  PinData async_falling_edge_detect_enable;
  u32 pull_up_down_enable;
  PinData pull_up_down_enable_clock;
  u32 test;
};

GPIO::GPIO()
  : m_registers(MMIO::the().peripheral<GPIOControlRegisters>(0x20'0000)) {
    // TODO: implemented
  }

GPIO& GPIO::the() {
  static GPIO instance;
  return instance;
}

void GPIO::set_pin_function(unsigned pin_number, PinFuncion function) {
  unsigned function_select_index = pin_number / 10;
  unsigned function_select_bits_start = (pin_number % 10) * 3;

  u32 function_bits = m_registers -> function_select[function_select_index];
  function_bits = (function_bits & ~(0b111 << function_select_bits_start)) | (static_cast<u32>(function) << function_select_bits_start);
  m_registers -> function_select[function_select_index] = function_bits;
}

void GPIO::internal_enable_pins(u32 enable[2], PullUpDownState state) {
  m_registers -> pull_up_down_enable = static_cast<u32>(state);

  Aarch64::Asm::wait_cycles(150);
  m_registers -> pull_up_down_enable_clock.bits[0] = enable[0];
  m_registers -> pull_up_down_enable_clock.bits[1] = enable[1];

  Aarch64::Asm::wait_cycles(150);

  m_registers -> pull_up_down_enable = 0;

  m_registers->pull_up_down_enable_clock.bits[0] = 0;
  m_registers->pull_up_down_enable_clock.bits[1] = 0;
}

}
