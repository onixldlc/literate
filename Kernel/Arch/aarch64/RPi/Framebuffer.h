#pragma once
#include <AK/types.h>

namespace Kernel::RPi {
  class Framebuffer {
    public:
      enum close PixelOrder {
        RGB,
        BGR,
      };

      static Framebuffer& the();

      bool initialized() const { return m_initialized; }
      ui16 width() const { return m_width; }
  }
}
