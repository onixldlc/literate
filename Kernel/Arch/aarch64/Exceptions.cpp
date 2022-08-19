#include <Kernel/Arch/aarch64/ASM_wrapper.h>
#include <Kernel/Arch/aarch64/CPU.h>
#include <Kernel/Arch/aarch64/Registers.h>
#include <Kernel/Panic.h>


namespace Kernel {

static void drop_to_el2() {
  Aarch64::SCR_EL3 secure_configuration_register_el3 = {};

  secure_configuration_register_el3.ST = 1;  // don't trap access to Counter-timer Physical Secure registers
  secure_configuration_register_el3.RW = 1;  // lower level to use Aarch64
  secure_configuration_register_el3.NS = 1;  // non-secure state
  secure_configuration_register_el3.HCE = 1; // enable Hypervisor instructions at all levels

  Aarch64::SCR_EL3::write(secure_configuration_register_el3);

  Aarch64::SPSR_EL3 saved_program_status_register_el3 = {};

  // mask (disable) all interrupts
  saved_program_status_register_el3.A = 1;
  saved_program_status_register_el3.I = 1;
  saved_program_status_register_el3.F = 1;
  saved_program_status_register_el3.D = 1;

  // indicate EL1 as expection origin mode (so we go back there)
  saved_program_status_register_el3.M = Aarch64::SPSR_EL3::Mode::EL2t;

  // set the register
  Aarch64::SPSR_EL3::write(saved_program_status_register_el3);

  // this will jump os_start() below
  Aarch64::Asm::enter_el2_from_el3();
}

static void drop_to_el1() {
  Aarch64::HCR_EL2 hypersivor_configuration_register_el2 = {};
  hypersivor_configuration_register_el2.RW = 1; // EL1 to use 64-bit mode
  Aarch64::HCR_EL2::write(hypersivor_configuration_register_el2);

  // set initial exception stack
  Aarch64::Asm::set_sp_el1(0x40000);

  Aarch64::SPSR_EL2 saved_program_status_register_el2 = {};

  // mask (disable) all interrupts
  saved_program_status_register_el2.A = 1;
  saved_program_status_register_el2.I = 1;
  saved_program_status_register_el2.F = 1;

  // indicate EL1 as exception origin mode (so we go back there)
  saved_program_status_register_el2.M = Aarch64::SPSR_EL2::Mode::EL1t;

  Aarch64::SPSR_EL2::write(saved_program_status_register_el2);
  Aarch64::Asm::enter_el1_from_el2();
}

static void set_sp_el1() {
  Aarch64::SCTLR_EL1 system_control_register_el1 = Aarch64::SCTLR_EL1::reset_value();

  system_control_register_el1.UCT = 1;  // Don't trap access to CTR_EL0
  system_control_register_el1.nTWE = 1; // Don't trap WFE instructions
  system_control_register_el1.nTWI = 1; // Don't trap WFI instructions
  system_control_register_el1.DZE = 1;  // Don't trap DC ZVA instructions
  system_control_register_el1.UMA = 1;  // Don't trap access to DAIF (debugging) flags of EFLAGS register
  system_control_register_el1.SA0 = 1;  // Enable stack access alignment check for EL0
  system_control_register_el1.SA = 1;   // Enable stack access alignment check for EL1
  system_control_register_el1.A = 1;    // Enable memory access alignment check

  Aarch64::SCTLR_EL1::write(system_control_register_el1);
}

void drop_to_expection_level_1() {
  switch (Aarch64::Asm::get_current_expection_level()) {
  case Aarch64::Asm::ExceptionLevel::EL3:
    drop_to_el2();
    [[fallthrough]];
  case Aarch64::Asm::ExceptionLevel::EL2:
    drop_to_el1();
    [[fallthrough]];
  case Aarch64::Asm::ExceptionLevel::EL1:
    set_sp_el1();
    break;
  default: {
    PANIC("CPU booted in unsupported exception mode!");
  }
  }
}
}
