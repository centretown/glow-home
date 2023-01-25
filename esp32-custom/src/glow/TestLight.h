#pragma once

#include "base.h"
#include "../esphome/core/color.h"

using esphome::Color;

namespace glow
{
  class TestLight
  {
  private:
    static const size_t MAX_LIGHT_SIZE = 4096;
    static Color colors[MAX_LIGHT_SIZE];

  public:
    Color &get(uint16_t index)
    {
      return colors[index % MAX_LIGHT_SIZE];
    }
  };

} // namespace glow
