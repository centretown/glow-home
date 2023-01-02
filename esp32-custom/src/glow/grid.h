#pragma once

#include <stdint.h>
#include <stdlib.h>

namespace glow
{
  struct Grid
  {
    uint16_t rows = 0;
    uint16_t columns = 0;
    uint16_t length = 0;

    uint16_t pivot_first = 0;
    uint16_t pivot_last = 0;
    uint16_t pivot_offset = 0;

    void setup(uint16_t full_length, uint16_t row_count);

    inline uint16_t map_columns(uint16_t i, div_t &point)
    {
      point = div(i, rows);
      return (uint16_t)(point.rem * columns + point.quot);
    }

    inline uint16_t map_diagonal_middle(uint16_t index)
    {
      div_t p = div(index - pivot_first, rows);
      return pivot_offset + p.quot +
             p.rem * (columns - 1);
    }

    uint16_t map_diagonal(uint16_t index);
    uint16_t map_diagonal_top(uint16_t index);
    uint16_t map_diagonal_bottom(uint16_t index);
  };

} // namespace glow
