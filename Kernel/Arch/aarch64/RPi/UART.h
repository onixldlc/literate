#pragma once

#include <AK/Types.h>

namespace Kernel::RPi {

struct UARTRegisters;

class UART {
public:
  static UART& the();

  void send(u32 c);
  u32 receive();

  void print_str(char const*, size_t);

private:
  UART();

  void set_baud_rate(int baud_rate, int uart_frequency_in_hz);
  void wait_until_we_can_send();
  void wait_until_we_can_receive();

  UARTRegisters volatile* m_registers;
};

}
