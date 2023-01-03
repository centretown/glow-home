#include "Grid.h"

namespace glow
{
  void Grid::setup(uint16_t full_length, uint16_t row_count, uint16_t org)
  {
    origin = org;
    length = full_length;
    rows = row_count;
    columns = length / rows;
    uint16_t lesser = (rows > columns) ? columns : rows;

    pivot_first = 0;
    pivot_last = 0;
    pivot_offset = 0;

    for (uint16_t i = 0; i < lesser; i++)
    {
      pivot_first += i;
    }

    pivot_offset = lesser - 1;
    pivot_last = pivot_first +
                 (columns - lesser) * rows + rows - 1;
  }

  uint16_t Grid::map_diagonal_top(uint16_t index)
  {
    uint16_t offset = 0;
    uint16_t start = 0;
    while (start < index)
    {
      offset++;
      start += offset;
    }

    if (start == index)
    {
      return offset;
    }

    start -= offset;
    offset--;
    return offset + (index - start) * (columns - 1);
  }

  uint16_t Grid::map_diagonal_bottom(uint16_t index)
  {
    uint16_t offset = 2 * columns - 1;
    uint16_t start = pivot_last + 1;
    uint16_t increment = rows;
    while (start < index)
    {
      offset += columns;
      increment--;
      start += increment;
    }

    if (start == index)
    {
      return offset;
    }

    start -= increment;
    offset -= columns;
    return offset + (index - start) * (columns - 1);
  }

  uint16_t Grid::map_diagonal(uint16_t index)
  {
    uint16_t offset = 0;

    if (columns < 3)
    {
      offset = index;
    }

    else if (index < pivot_first)
    {
      offset = map_diagonal_top(index);
    }

    else if (index <= pivot_last)
    {
      offset = map_diagonal_middle(index);
    }

    else
    {
      offset = map_diagonal_bottom(index);
    }

    if (origin == 1)
    {
      div_t point = div(offset, columns);
      offset = point.quot * columns +
               (columns - point.rem - 1);
    }

    return offset;
  }
}