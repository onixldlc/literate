#pragma once
#define PAGE_MASK (~(FlatPtr)0xfffu)

namespace Kernel {
void drop_to_exception_level_1();
void init_page_tables();
}
