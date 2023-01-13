#pragma once

#include "base.h"

namespace glow
{
  enum Origin : uint8_t
  {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
  };

  enum Orientation : uint8_t
  {
    Horizontal,
    Vertical,
    Diagonal
  };

  struct Pivot
  {
    uint16_t first = 0;
    uint16_t last = 0;
    uint16_t offset = 0;
  };

  class Grid
  {
  private:
  public:
    uint16_t rows = 0;
    uint16_t columns = 0;
    uint16_t length = 0;

    Origin origin = TopLeft;
    Orientation orientation = Horizontal;

    div_t point{0, 0};
    Pivot pivot;

  public:
    void setup(uint16_t full_length, uint16_t row_count,
               Origin org = TopLeft,
               Orientation ori = Horizontal);

    uint16_t First() const ALWAYS_INLINE
    {
      return pivot.first;
    }
    uint16_t Last() const ALWAYS_INLINE
    {
      return pivot.last;
    }
    uint16_t Offset() const ALWAYS_INLINE
    {
      return pivot.offset;
    }

    uint16_t map(uint16_t index);
    uint16_t map_diagonal(uint16_t index);
    uint16_t map_diagonal_top(uint16_t index);
    uint16_t map_diagonal_bottom(uint16_t index);
    uint16_t map_to_origin(uint16_t offset);

    uint16_t map_columns(uint16_t i, div_t &point) ALWAYS_INLINE
    {
      point = div(i, rows);
      return (uint16_t)(point.rem * columns + point.quot);
    }

    uint16_t map_diagonal_middle(uint16_t index) ALWAYS_INLINE
    {
      div_t p = div(index - pivot.first, rows);
      return pivot.offset + p.quot +
             p.rem * (columns - 1);
    }

    void log_buffer(char *buffer, size_t buffer_size) const
    {
      snprintf(buffer, buffer_size,
               "rows=%u columns=%u length=%u first=%u last=%u offset=%u",
               rows, columns, length,
               pivot.first, pivot.last, pivot.offset);
    }
  };

} // namespace glow
