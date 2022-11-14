#pragma once

#include "Range.h"

namespace glow
{
  enum : uint8_t
  {
    GRID_ROWS = 0,
    GRID_COLUMNS = 1,
  };

  class Grid : public Range
  {
  protected:
    uint16_t columns = 1;
    uint16_t rows = 1;
    uint8_t profile = GRID_ROWS;

  public:
    Grid(uint16_t first = 0, uint16_t last = 1,
         uint16_t row_count = 1,
         uint8_t profile_ = GRID_ROWS)

    {
      profile = profile_;
      Resize(first, last, row_count);
    }

    // access
    inline uint8_t Profile() const { return profile; }
    inline uint16_t Rows() const { return rows; }
    inline uint16_t Columns() const { return columns; }

    // modify
    inline uint8_t Profile(uint8_t v) { return profile = v; }

    inline uint32_t Resize(uint16_t first, uint16_t last, uint16_t row_count)
    {
      Range::Resize(first, last);
      rows = row_count;
      columns = Length() / rows;
      return Pack();
    }

    // implement
    inline uint16_t Map(uint16_t index)
    {
      if (profile == GRID_ROWS)
      {
        return index;
      }

      index -= Begin();
      div_t point = div(index, rows);
      return Begin() + point.rem * columns + point.quot;
    }
  };
} // namespace glow
