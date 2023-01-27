#pragma once

#include "base.h"
#include "Properties.h"

namespace glow
{
  struct Pivot
  {
    uint16_t first = 0;
    uint16_t last = 0;
    uint16_t offset = 0;
  };

  struct Coordinates
  {
    uint16_t row;
    uint16_t column;
  };

  class Grid
  {
  private:
  public:
    uint16_t rows = 0;
    uint16_t columns = 0;
    uint16_t length = 0;

    uint8_t origin = Properties::TopLeft;
    uint8_t orientation = Properties::Horizontal;

    div_t point{0, 0};
    Pivot pivot;

  public:
    void setup(Properties &properties);

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

    Coordinates map_coordinates(uint16_t offset)
    {
      Coordinates coord{0, 0};
      div_t result = div(offset, columns);
      return Coordinates{static_cast<uint16_t>(result.quot),
                         static_cast<uint16_t>(result.rem)};
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

    void log_buffer(char *buffer, size_t buffer_size) const;
  };

} // namespace glow
