#include <Kernel/Arch/Processor.h>
#include <Kernel/KSyms.h>
#include <Kernel/Panic.h>


using namespace Kernel;

[[noreturn]] void __assertion_failed(char const* msg, char const* file, unsigned line, char const* func) {
  critical_dmesgln("ASSERTION FAILED: {}", msg);
  critical_dmesgln("{}:{} in {}", file, line, func);

  // used for printing a nice backtrace
  PANIC("Aborted");
}

void Kernel::__panic(char const* file, unsigned int line, char const* function) {
  critical_dmesgln("at {}:{} in {}", file, line, function);
  dump_backtrace(PrintToScreen::Yes);

  Processor::halt();
}
