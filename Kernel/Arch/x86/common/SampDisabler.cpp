#include <Kernel/Arch/SmapDisabler.h>

#include <Kernel/Arch/x86/ASM_wrapper.h>

namespace Kernel {
SmapDisabler::SmapDisabler()
  : m_flags(cpu_flags()) {
    stac();
  }

SmapDisabler::~SmapDisabler() {
  if (!(m_flags & 0x40000))
    clac();
}
}
