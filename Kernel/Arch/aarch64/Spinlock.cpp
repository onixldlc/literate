#include <Kernel/Arch/Spinlock.h>

namespace Kernel {

u32 Spinlock::lock() {
  return 0;
}

void Spinlock::unlock(u32) {
  // TODO: Implemented
}

u32 RecursiveSpinlock::lock() {
  return 0;
}

void RecursiveSpinlock::unlock(u32) {
  // TODO: Implemented
}

}
