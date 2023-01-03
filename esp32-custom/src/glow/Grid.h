#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

namespace glow
{
  class Grid
  {
  private:
    uint16_t rows = 0;
    uint16_t columns = 0;
    uint16_t length = 0;

    uint16_t pivot_first = 0;
    uint16_t pivot_last = 0;
    uint16_t pivot_offset = 0;

    uint16_t origin = 0;

  public:
    void setup(uint16_t full_length, uint16_t row_count, uint16_t org = 0);

    inline uint16_t Length() const
    {
      return length;
    }
    inline uint16_t Rows() const
    {
      return rows;
    }
    inline uint16_t Columns() const
    {
      return columns;
    }

    inline uint16_t First() const
    {
      return pivot_first;
    }
    inline uint16_t Last() const
    {
      return pivot_last;
    }
    inline uint16_t Offset() const
    {
      return pivot_offset;
    }

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

    void log_buffer(char *buffer, size_t buffer_size) const
    {
      snprintf(buffer, buffer_size,
               "rows=%u columns=%u length=%u first=%u last=%u offset=%u",
               rows, columns, length,
               pivot_first, pivot_last, pivot_offset);
    }
  };

} // namespace glow
