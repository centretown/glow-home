#pragma once

#include "base.h"
#include "../esphome/core/color.h"

using esphome::Color;

namespace glow
{
  class HostLight
  {
  private:
    static const size_t MAX_LIGHT_SIZE = 4096;
    Color colors[MAX_LIGHT_SIZE] = {{0, 0, 0}};

  public:
    Color &get(uint16_t index) ALWAYS_INLINE
    {
      return colors[index % MAX_LIGHT_SIZE];
    }
  };

} // namespace glow
