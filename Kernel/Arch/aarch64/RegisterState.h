#pragm once

namespace Kernel {
struct RegisterState {
  FlatPtr userspace_sp() const { return 0; }
  FlatPtr ip() const { return 0; }
};

struct DebugRegisterState {
};

}
