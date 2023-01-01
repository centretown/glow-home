#include <iostream>
#include "../include/grid.h"

namespace glow
{
  Grid::Grid(uint16_t rows, uint16_t columns)
      : rows(rows), columns(columns)
  {
    count = columns + rows - 1;
    length = rows * columns;
    uint16_t lesser = (rows > columns) ? columns : rows;

    for (uint16_t i = 0; i < lesser; i++)
    {
      pivot_first += i;
    }
    pivot_offset = lesser - 1;
    pivot_last = pivot_first +
                 (columns - lesser) * rows + rows - 1;
  }

  uint16_t Grid::map_diagonal_middle(uint16_t index)
  {
    div_t p = div(index - pivot_first, rows);
    return pivot_offset + p.quot +
           p.rem * (columns - 1);
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
    if (columns < 3)
    {
      return index;
    }

    if (index < pivot_first)
    {
      return map_diagonal_top(index);
    }

    if (index <= pivot_last)
    {
      return map_diagonal_middle(index);
    }

    return map_diagonal_bottom(index);
  }
}