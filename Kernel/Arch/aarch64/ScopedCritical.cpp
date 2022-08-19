#include <Kernel/Arch/ScopedCritical.h>

#include <Kernel/Arch/Processor.h>

namespace Kernel {

ScopedCritical::ScopedCritical() = default;
ScopedCritical::~ScopedCritical() = default;

ScopedCritical::ScopedCritical(ScopedCritical& /* from */) {
  VERIFY_NOT_REACHED();
}

ScopedCritical& ScopedCritical::operator=(ScopedCritical&&) {
  VERIFY_NOT_REACHED();
  return *this;
}

void ScopedCritical::leave() {
  VERIFY_NOT_REACHED();
}

void ScopedCritical::enter() {
  VERIFY_NOT_REACHED();
}

}
