#pragma once

#include <AK/Types.h>

namespace Kernel {
class BootPPMParser {
public:
  struct {
    u32 width = 0;
    u32 height = 0;
    u8 const* pixel_data = nullptr;
  } image;
  BootPPMParser(u8 const* buffer, u32 buffer_size);

  bool parse();

private:
  char const* m_cursor;
  char const* m_buffer_end;

  bool check_position() const;
  bool parse_magic();
  bool parse_new_line();
  bool parse_comment();
  bool parse_integer(u32& value);
};

}
