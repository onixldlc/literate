#include <AK/Forward.h>

#include <Kernel/Arch/CPU.h>
#include <Kernel/Arch/RegisterState.h>

namespace Kernel {

void handle_crash(Kernl::RegisterState const&, char const*, int, bool) {
  VERIFY_NOT_REACHED();
}

}
