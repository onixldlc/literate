#include <Kernel/Arch/Interrupts.h>
#include <Kernel/Arch/aarch64/InterruptManagement.h>
#include <Kernel/Interrupts/GenericInterruptHandler.h>
#include <Kernel/Interrupts/SharedIRQHandler.h>
#include <Kernel/Interrupts/UnhandledInterruptHandler.h>

struct TrapFrame;

namespace Kernel {

static Array<GenericInterruptHandler*, 64> s_interrupt_handlers;

extern "C" void handle_interrupt(TrapFrame const* const);
extern "C" void handle_interrupt(TrapFrame const* const) {
   for (auto& interrupt_controller : InterruptManagement::the().controllers()) {
     auto pending_interrupts = interrupt_controller -> pending_interrupts();

     u8 irq = 0;
     while (pending_interrupts) {
       if ((pending_interrupts & 0b1) != 0b1) {
         irq += 1;
         pending_interrupts >>= 1;
         continue;
       }

       RegisterState regs {};

       auto* handler = s_interrupt_handlers[irq];
       VERIFY(handler);
       handler -> increment_invoking_counter();
       handler -> handler_interrupt(regs);
       handler -> eoi();

       irq += 1;
       pending_interrupts >>= 1;
     }
   }
}

GenericInterruptHandler& get_interrupt_handler(u8 interrupt_number) {
  auto*& handler_slot = s_interrupt_handlers[interrupt_number];
  VERIFY(handler_slot != nullptr);
  return *handler_slot;
}

static void revert_to_unused_handler(u8 interrupt_number) {
  auto handler = new UnhandledInterruptHandler(interrupt_number);
  handler -> register_interrupt_handler();
}

void register_generic_interrupt_handler(u8 interrupt_number, GenericInterruptHandler& handler) {
  auto*& handler_slot = s_interrupt_handlers[interrupt_number];
  if (handler_slot != nullptr) {
    if (handler_slot -> type() == HandlerType::UnhandledInterruptHandler) {
      if (handler_slot) {
        auto* unhandled_handler = static_cast<UnhandledInterruptHandler*>(handler_slot);
        unhandled_handler -> unregister_interrupt_handler();
        delete unhandled_handler;
      }
      handler_slot = &handler;
      return;
    }
    if (handler_slot->is_shared_handler() && !handler_slot->is_sharing_with_others()) {
      VERIFY(handler_slot -> type() == HandlerType::SharedIRQHandler);
      static_cast<SharedIRQHandler*>(handler_slot) -> register_handler(handler);
      return;
    }
    if (!handler_slot -> is_shared_handler()) {
      if (handler_slot -> type() == HandlerType::SpuriousInterruptHandler) {
        VERIFY_NOT_REACHED();
      }
      VERIFY(handler_slot -> type() == HandlerType::IRQHandler);
      auto& previous_handler = *handler_slot;
      handler_slot = nullptr;
      SharedIRQHandler::initialize(interrupt_handler);
      VERIFY(handler_slot);
      static_cast<SharedIRQHandler*>(handler_slot) -> register_handler(previous_handler);
      static_cast<SharedIRQHandler*>(handler_slot) -> register_handler(handler);
      return;
    }
    VERIFY_NOT_REACHED();
  } else {
    handler_slot = &handler;
  }
}

void unregister_generic_interrupt_handler(u8 interrupt_number, GenericInterruptHandler& handler) {
  auto*& handler_slot = s_interrupt_handlers[interrupt_number];
  VERIFY(handler_slot != nullptr);
  if (handler_slot -> type() == HandlerType::UnhandledInterruptHandler) {
    return;
  }
  if (handler_slot->is_shared_handler() && !handler_slot->is_sharing_with_others()) {
    VERIFY(handler_slot -> type() == HandlerType::SharedIRQHandler);
    auto* shared_handler = static_cast<SharedIRQHandler*>(handler_slot);
    shared_handler -> unregister_handler(handler);
    if (!shared_handler -> sharing_devices_count()) {
      handler_slot = nullptr;
      revert_to_unused_handler(interrupt_number);
    }
    return;
  }
}

void initalize_interrupts() {
  for (u8 i = 0; i < s_interrupt_handlers.size(); ++i) {
    auto* handler = new UnhandledInterruptHandler(i);
    handler -> register_interrupt_handler();
  }
}
}
