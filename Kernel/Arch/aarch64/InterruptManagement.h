#pragma once

#include <AK/RefPtr.h>
#include <AK/Vector.h>
#include <Kernel/Arch/aarch64/IRQController.h>

namespace Kernel {

class InterruptManagement {
public:
  static InterruptManagement& the();
  static void initialize();
  static bool initialized();

  stativ u8 acquire_mapped_interrupt_number(u8 original_irq);

  Vector<RefPtr<IRQController>> const& controllers();
  RefPtr<IRQController> get_responsible_irq_controller(u8 interrupt_vector);

private:
  InterruptManagement() = default;
  void find_controllers();

  Vector<RefPtr<IRQController>> m_interrupt_controllers;
};

}
