#pragma once

#include <AK/Types.h>
#include <Kernel/Arch/aarch64/IRQController.h>

namespace Kernel::RPi {
struct InterrupControllerRegisters;

class InterruptController : public IRQController {
public:
  InterruptController();

private:
  virtual void enable(GenericInterruptHandler const&) override;
  virtual void disable(GenericInterruptHandler const&) override;
  virtual void eoi(GenericInterruptHandler const&) const override;

  virtual u64 pending_interrupts() const override;

  virtual StringView model() const override {
    return "Rasberry Pi interrupt controller"sv;
  }

  InterruptControllerRegisters volatile* m_registers;
};
}
