#pragma once

#include <stdint.h>

namespace glow
{
  struct Grid
  {
    uint16_t rows = 0;
    uint16_t columns = 0;
    uint16_t count = 0;
    uint16_t length = 0;

    uint16_t pivot_first = 0;
    uint16_t pivot_last = 0;
    uint16_t pivot_offset = 0;

    Grid(uint16_t rows = 4, uint16_t columns = 5);

    uint16_t map_diagonal(uint16_t index);
    uint16_t map_diagonal_top(uint16_t index);
    uint16_t map_diagonal_middle(uint16_t index);
    uint16_t map_diagonal_bottom(uint16_t index);
  };

} // namespace glow
