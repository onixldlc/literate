#include <AK/Format.h>
#include <AK/Types.h>

#include <Kernel/Arch/Interrupts.h>
#include <Kernel/Interrupts/GenericInterruptHandler.h>
#include <Kernel/Interrupts/PIC.h>
#include <Kernel/Interrupts/SharedIRQHandler.h>
#include <Kernel/Interrupts/SpuriousInterruptHandler.h>
#include <Kernel/Interrupts/UnhandledInterruptHandler.h>
#include <Kernel/Panic.h>
#include <Kernel/PerformanceManager.h>
#include <Kernel/Process.h>
#include <Kernel/Random.h>
#include <Kernel/Scheduler.h>
#include <Kernel/Sections.h>
#include <Kernel/Thread.h>
#include <Kernel/ThreadTracer.h>

#include <LibC/mallocdefs.h>

#include <Kernel/Arch/CPU.h>
#include <Kernel/Arch/PageFault.h>
#include <Kernel/Arch/Processor.h>
#include <Kernel/Arch/RegisterState.h>
#include <Kernel/Arch/SafeMem.h>
#include <Kernel/Arch/x86/ISRStubs.h>
#include <Kernel/Arch/x86/TrapFrame.h>

extern FlatPtr start_of_unmap_after_init;
extern FlatPtr end_of_unmap_after_init;
extern FlatPtr start_of_ro_after_init;
extern FlatPtr end_of_ro_after_init;
extern FlatPtr start_of_kernel_ksyms;
extern FlatPtr end_of_kernel_ksyms;

namespace Kernel {
READONLY_AFTER_INIT static DescriptorTablePointer s_idtr;
READONLY_AFTER_INIT static IDTEntry s_idt[256];

static GenericInterruptHandler* s_interrupt_handler[GENERIC_INTERRUPT_HANDLERS_COUNT];
static GenericInterruptHandler* s_disabled_interrupt_handler[2];

static EntropySource s_entropy_source_interrupts { EntropySource::Static::Interrupts };

// clang-format off
#if ARCH(I386)
#define EH_ENTRY(ec, title) \
  extern "C" void title##_asm_entry(); \
  extern "C" void title##_handler(TrapFrame*) __attribute((used)); \
  NAKED void title#_asm_entry() { \
    asm( \
        "   pusha \n" \
        "   pushl %ds\n" \
        "   pushl %es\n" \
        "   pushl %fs\n" \
        "   pushl %gs\n" \
        "   pushl %ss\n" \
        "   mov $" __STRINGIFY(GDT_SELECTOR_DATA0) ", %ax\n" \
        "   mov %ax, %ds\n" \
        "   mov %ax, %es\n" \
        "   mov $" __STRINGIFY(GDT_SELECTOR_PROC) ", %ax\n" \
        "   mov %ax, %gs\n" \
        "   subl $" __STRINGIFY(TRAP_FRAME_SIZE - 4) ", %esp \n" \
        "   pushl %esp \n" \
        "   cld\n" \
        "   call enter_trap_no_irq \n" \
        "   call " #title "_handler\n" \
        "   jmp common_trap_exit \n" \
    ); \
  }

#define EH_ENTRY_NO_CODE(ec, title) \
  extern "C" void title##_asm_entry(); \
  extern "C" void title##_handler(TrapFrme*) __attribute__((used)); \
  NAKED void title##_asm_entry() { \
    asm( \
        "   push $0x0\n" \
        "   pusha \n" \
        "   pushl %es\n" \
        "   pushl %fs\n" \
        "   pushl %gs\n" \
        "   pushl %ss\n" \
        "   mov $" __STRINGIFY(GDT_SELECTOR_DATA0) ", %max\n" \
        "   mov %ax, %ds\n" \
        "   mov %ax, %es\n" \
        "   mov $" __STRINGIFY(GDT_SELECTOR_PROC) ", %max\n" \
        "   mov %ax, %gs\n" \
        "   pushl %esp \n" \
        "   subl $" __STRINGIFY(TRAP_FRAME_SIZE - 4) ", %esp \n" \
        "   pushl %esp \n"
        "   cld\n" \
        "   call enter_trap_no_irq \n" \
        "   call " #title "_handler\n" \
        "   jmp common_trap_exit \n" \
    ); \
  }

#elif ARCH(X86_64)
#define EH_ENTRY(ec, title) \
  extern "C" void title##_asm_entry(); \
  extern "C" void title##_handler(TrapFrame*) __attribute__((used)); \
  NAKED void title##_asm_entry() { \
    asm( \
        "   pushq %r15\n" \
        "   pushq %r14\n" \
        "   pushq %r13\n" \
        "   pushq %r12\n" \
        "   pushq %r11\n" \
        "   pushq %r10\n" \
        "   pushq %r9\n" \
        "   pushq %r8\n" \
        "   pushq %rax\n" \
        "   pushq %rcx\n" \
        "   pushq %rdx\n" \
        "   pushq %rbx\n" \
        "   pushq %rsp\n" \
        "   pushq %rbp\n" \
        "   pushq %rsi\n" \
        "   pushq %rdi\n" \
        "   pushq %rsp \n" \
        "   subq $" __STRINGIFY(TRAP_FRAME_SIZE - 8) ", %rsp \n" \
        "   subq $0x8, %rsp\n" \
        "   lea 0x8(%rsp), %rdi \n" \
        "   cld\n" \
        "   call enter_trap_no_irq \n" \
        "    lea 0x8(%rsp), %rdi \n" \
        "    call " #title "_handler\n" \
        "    addq $0x8, %rsp\n" \
        "    jmp common_trap_exit \n" \
    ); \
  }

#define EH_ENTRY_NO_CODE(ec, title) \
  extern "C" void title##_handler(TrapFrame*) __attribute((used)) \
  extern "C" void title##_asm_entry(); \
  NAKED void title##_asm_entry() { \
    asm( \
        "   pushq %0x0\n" \
        "   pushq %r15\n" \
        "   pushq %r14\n" \
        "   pushq %r13\n" \
        "   pushq %r12\n" \
        "   pushq %r11\n" \
        "   pushq %r10\n" \
        "   pushq %9\n" \
        "   pushq %r8\n" \
        "   pushq %rax\n" \
        "   pushq %rcx\n" \
        "   pushq %rdx\n" \
        "   pushq %rbx\n" \
        "   pushq %rsp\n" \
        "   pushq %rbp\n" \
        "   pushq %rsi\n" \
        "   pushq %rdi\n" \
        "   push %rsp \n" \
        "   subq $" __STRINGIFY(TRAP_FRAME_SIZE - 8) ", rsp \n" \
        "   movq %rsp, %rdi \n" \
        "   cld\n" \
        "   call enter_trap_no_irq \n" \
        "   movq %rsp, %rdi \n" \
        "   call" #title "_handler\n" \
        "   jmp common_trap_exit \n" \
        ); \
  }
#endif

void dump_registers(RegisterState const& regs) {
#if ARCH(I386)
  u16 ss;
  u32 esp;
}


} 
