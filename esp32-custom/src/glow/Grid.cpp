#include "Grid.h"

namespace glow
{
  void Grid::setup(uint16_t full_length, uint16_t row_count,
                   Origin org,
                   Orientation ori)
  {
    length = full_length;
    rows = row_count;
    origin = org;
    orientation = ori;
    columns = length / rows;
    uint16_t lesser = (rows > columns) ? columns : rows;

    pivot.first = 0;
    pivot.last = 0;
    pivot.offset = 0;

    for (uint16_t i = 0; i < lesser; i++)
    {
      pivot.first += i;
    }

    pivot.offset = lesser - 1;
    pivot.last = pivot.first +
                 (columns - lesser) * rows + rows - 1;
  }

  uint16_t Grid::map(uint16_t index)
  {
    uint16_t offset = index;
    if (orientation == Diagonal)
    {
      offset = map_diagonal(index);
    }
    else if (orientation == Vertical)
    {
      offset = map_columns(index, point);
    }
    return map_to_origin(offset);
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
    uint16_t start = pivot.last + 1;
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

    if (index < pivot.first)
    {
      return map_diagonal_top(index);
    }

    if (index <= pivot.last)
    {
      return map_diagonal_middle(index);
    }

    return map_diagonal_bottom(index);
  }

  uint16_t Grid::map_to_origin(uint16_t offset)
  {
    if (origin == BottomRight)
    {
      return length - offset - 1;
    }

    point = div(offset, columns);

    if (origin == BottomLeft)
    {
      return (rows - point.quot - 1) * columns +
             point.rem;
    }

    if (origin == TopRight)
    {
      return point.quot * columns +
             (columns - point.rem - 1);
    }

    return offset;
  }

}