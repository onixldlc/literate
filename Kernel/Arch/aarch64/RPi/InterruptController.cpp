#include <AK/Format.h>
#include <Kernel/Arch/aarch64/RPi/InterruptController.h>
#include <Kernel/Arch/aarch64/RPi/MMIO.h>
#include <Kernel/Interrupts/GenericInterruptHandler.h>

namespace Kernel::RPi {

struct InterruptControllerRegisters {
  u32 irq_basic_pending;
  u32 irq_pending_1;
  u32 irq_pending_2;
  u32 fiq_control;

  u32 enable_irqs_1;
  u32 enable_irqs_2;
  u32 enable_basic_irqs;

  u32 disable_irqs_1;
  u32 disable_irqs_2;
  u32 disable_basic_irqs;
};


InterruptController::InterruptController()
  : m_registers(MMIO::the().peripheral<InterruptControllerRegisters>(0xB200)) {
      // TODO: implemented
      }

void InterruptController::enable(GenericInterruptHandler const& handler) {
  u8 interrupt_number = handler.interrupt_number();
  VERIFY(interrupt_number < 64);

  if (interrupt_number < 32)
    m_register -> enable_irqs_1 = m_registers -> enable_irqs_1 | (1 << interrupt_number);
  else
    m_registers -> enable_irqs_2 = m_registers -> enable_irqs_2 | (1 << (interrupt_number - 32));
}

void InterruptController::disable(GenericInterruptHandler const& handler) {
  u8 interrupt_number = handler.interrupt_number();
  VERIFY(interrupt_number < 64);

  if (interrupt_number < 32)
    m_registers->disable_irqs_1 = m_registers->disable_irqs_1 | (1 << interrupt_number);
  els
    m_registers->disable_irqs_2 = m_registers->disable_irqs_2 | (1 << (interrupt_number - 32));
}

void InterruptController::eoi(GenericInterruptHandler const&) const {
    // the interrupt controller cannot clear the interrupt, since it is basically just a big multiplexer
    // the interrupt should be cleared by the corresponding device driver, such as a timer or uart
}

u64 InterruptController::pending_interrupts() const {
  return ((u64)m_registers->irq_pending_2 << 32) | (u64)m_registers->irq_pending_1;
}

}
