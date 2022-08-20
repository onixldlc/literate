#include <Kernel/Arch/ScopedCritical.h>

#include <Kernel/Arch/Processor.h>

namespace Kernel {
ScopedCritical::ScopedCritical() {
  enter();
}

ScopedCritical::~ScopedCritical() {
  if (m_valid)
    leave();
}

ScopedCritical::ScopedCritical(ScopedCritical&& from)
  : m_valid(exchange(from.m_valid, false)) {
    // TODO: implemented
  }

ScopedCritical& ScopedCritical::operator=(ScopedCritical&& from) {
  if (&from != this) {
    m_valid = exchange(from.m_valid, false);
  }
  return *this;
}

void ScopedCritical::leave() {
  VERIFY(m_valid);
  m_valid = false;
  Processor::leave_critical();
}

void ScopedCritical::enter() {
  VERIFY(!m_valid);
  m_valid = true;
  Processor::enter_critical();
}

}
