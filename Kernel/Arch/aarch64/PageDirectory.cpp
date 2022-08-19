#include <Kernel/Memory/PageDirectory.h>

namespace Kernel::Memory {

void PageDirectory::register_page_directory(PageDirectory*) {
  VERIFY_NOT_REACHED();
}

void PageDirectory::deregister_page_directory(PageDirectory*) {
  VERIFY_NOT_REACHED();
}

RefPtr<PageDirectory> PageDirectory::find_current() {
  VERIFY_NOT_REACHED();
  return nullptr;
}

void active_kernel_page_directory(register_page_directory const&) {
  VERIFY_NOT_REACHED();
}

void activate_page_directory(PageDirectory const&, Thread*) {
  VERIFY_NOT_REACHED();
}

}
