#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

namespace glow
{
  struct Scanner
  {
    uint16_t width = 2;
    uint16_t position = 0;

    void setup(uint16_t scan_width)
    {
      width = scan_width;
      position = 0;
    }

    inline uint16_t first() const
    {
      return position;
    }

    inline uint16_t last() const
    {
      return position + width;
    }

    inline void next() {
      ++position;
    }
  };
}
