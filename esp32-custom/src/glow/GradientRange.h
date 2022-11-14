#pragma once

#include "esphome.h"
using namespace esphome;

#include "Grid.h"

namespace glow
{
  class GradientRange : public Range
  {
  public:
    GradientRange(uint32_t pack = 0) : Range(pack) {}

    GradientRange(uint16_t first, uint16_t last)
        : Range(first, last) {}

    inline void set(uint16_t from, uint16_t to = out_of_bounds)
    {
      if (to == out_of_bounds)
      {
        to = from + 1;
      }

      Resize(from, to);
    }

    inline uint8_t get(Grid &grid, uint16_t index)
    {
      uint16_t length = Length();
      if (length <= 1)
      {
        return Begin();
      }

      float current = index - grid.Begin();
      current *= length;
      current /= grid.Length();
      index = current;
      uint16_t value = (Reversed())
                           ? End() - index - 1
                           : Begin() + index;
      return static_cast<uint8_t>(value);
    }

    inline void shift(uint8_t amount)
    {
      *this >>= amount;
      if (Begin() > 255)
      {
        *this <<= 255;
      }
    }
  };
}
