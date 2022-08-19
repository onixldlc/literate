#pragma once

#include <AK/Types.h>

#include <AK/Platform.h>

namespace Kernel {
struct TrapFrame {
  u64 x[31]; // saved general purpose register
  u64 spsr_el1; // save processor status register, EL1
  u64 elr_el1; // exception link register, EL1
  u64 tpidr_el1; // EL0 thread ID
  u64 sp_el0; // EL0 stack pointer
};

}
