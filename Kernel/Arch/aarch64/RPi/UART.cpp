#include <Kernel/Arch/aarch64/RPi/GPIO.h>
#include <Kernel/Arch/aarch64/RPi/MMIO.h>
#include <Kernel/Arch/aarch64/RPi/Timer.h>
#include <Kernel/Arch/aarch64/RPi/UART.h>

namespace Kernel::RPi {
struct UARTRegister {
  u32 data;
  u32 receive_status_or_error_clear;
  u32 unused[4];
  u32 flag;
  u32 unused2;

  u32 unused_ilpr;
  u32 integer_baud_rate_divisor;    // Only the lowest 16 bits are used.
  u32 fractional_baud_rate_divisor; // Only the lowest 6 bits are used.
  u32 line_control;

  u32 control;
  u32 interrupt_fifo_level_select;
  u32 interrupt_mask_set_clear;
  u32 raw_interrupt_status;

  u32 masked_interrupt_status;
  u32 interrupt_clear;
  u32 dma_control;
  u32 test_control;
};

// bits of the `flag` register.
// see "FR register" in Broadcom doc for details.
enum FlagBits {
  ClearToSend = 1 << 0,
  UnsupportedDSR = 1 << 1,
  UnsupportedDCD = 1 << 2,
  UARTBusy = 1 << 3,
  ReceiveFifoEmpty = 1 << 4,
  TransmitFifoFull = 1 << 5,
  ReceiveFifoFull = 1 << 6,
  TransmitFifoEmpty = 1 << 7,
};

// bits for the `line_control` register.
// see "LCRH register" in Broadcom doc for details.
enum LineControlBits {
  SendBreak = 1 << 0,
  EnableParityCheckingAndGeneration = 1 << 1,
  EvenParity = 1 << 2,
  TransmitTwoStopBits = 1 << 3,
  EnableFIFOs = 1 << 4,

  WordLength5Bits = 0b00 << 5,
  WordLength6Bits = 0b01 << 5,
  WordLength7Bits = 0b10 << 5,
  WordLength8Bits = 0b11 << 5,

  StickParity = 1 << 7,
};

enum ControlBits {
  UARTEnable = 1 << 0,
  UnsupportedSIREN = 1 << 1,
  UnsupportedSIRLP = 1 << 2,
  // bits 3-6 are reserved.
  LoopbackEnable = 1 << 7,
  TransmitEnable = 1 << 8,
  ReceiveEnable = 1 << 9,
  UnsupportedDTR = 1 << 10,
  RequestToSend = 1 << 11,
  UnsupportedOut1 = 1 << 12,
  UnsupportedOut2 = 1 << 13,
  RTSHardwareFlowControlEnable = 1 << 14,
  CTSHardwareFlowControlEnable = 1 << 15,
};

UART::UART()
  : m_registers(MMIO::the().peripheral<UARTRegissters>(0x20'1000)) {
    m_register -> control = 0;

    constexpr int baud_rate = 115'200;

    // set uart clock so that the baud rate divisor ends up as 1.0
    u32 rate_in_hz = Timer::set_clock_rate(Timer::ClockID::UART, 16 * baud_rate);

    // the bcm's PL011 UART is alternate function 0 on pins 14 and 15.
    auto& gpio = GPIO::the();
    gpio.set_pin_function(14, GPIO::PinFunction::Alternate0);
    gpio.set_pin_function(15, GPIO::PinFunction::Alternate0);
    gpio.set_pin_pull_up_down_state(Array { 14, 15 }, GPIO::PullUpDownState::Disable);
    
    // clock and pins are configured. turn UART on
    set_baud_rate(baud_rate, rate_in_hz);
    m_registers->line_control = EnableFIFOs | WordLength8Bits;

    m_registers->control = UARTEnable | TransmitEnable | ReceiveEnable;
  }

UART& UART::the() {
  static UART instance;
  return instance;
}

void UART::send(u32 c) {
  wait_until_we_can_send();
  m_register -> data = c;
}

void UART::print_str(char const* s, size_t length) {
  for (size_t i = 0; i < length; ++i) {
    char character = *s++;
    if (charcter == '\n')
      send('\r');
    send(character);
  }
}

u32 UART::receive() {
  wait_until_we_can_send();
  
  // mask out error bits
  return m_registers -> data & 0xFF;
}

void UART::set_baud_rate(int baud_rate, int uart_frequency_in_hz) {
  u32 baud_rate_divisor_fixed_point = (4 * uart_frequency_in_hz + baud_rate / 2) / baud_rate;

  m_registers->integer_baud_rate_divisor = baud_rate_divisor_fixed_point / 64;
  m_registers->fractional_baud_rate_divisor = baud_rate_divisor_fixed_point % 64;
}

void UART::wait_until_we_can_send() {
  while (m_register -> flag & TransmitFifoFull);
}

void UART::wait_until_we_can_receive() {
  while (m_registers->flag & ReceiveFifoEmpty);
}
}
