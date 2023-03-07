#include "Grid.h"
#include <math.h>

namespace glow
{
#ifndef MICRO_CONTROLLER

  std::string Grid::make_code()
  {
    std::stringstream s;
    s << "{" << length << ","
      << rows << ","
      << origin << ","
      << orientation << "}";
    return s.str();
  }

  std::string Grid::keys[Grid::KEY_COUNT] = {
      "length",
      "rows",
      "origin",
      "orientation",
  };

  std::string Grid::origin_keys[ORIGIN_COUNT] = {
      "top left",
      "top right",
      "bottom left",
      "bottom right",
  };

  std::unordered_map<std::string, uint16_t> Grid::origin_map = {
      {origin_keys[TopLeft], TopLeft},
      {origin_keys[TopRight], TopRight},
      {origin_keys[BottomLeft], BottomLeft},
      {origin_keys[BottomRight], BottomRight},
  };

  std::string Grid::orientation_keys[ORIENTATION_COUNT] = {
      "horizontal",
      "vertical",
      "diagonal",
  };

  std::unordered_map<std::string, uint16_t> Grid::orientation_map = {
      {orientation_keys[Horizontal], Horizontal},
      {orientation_keys[Vertical], Vertical},
      {orientation_keys[Diagonal], Diagonal},
  };

#endif
  uint16_t Grid::adjust_bounds(float bound)
  {
    uint16_t scaled = static_cast<uint16_t>(round(bound));
    if (orientation == Horizontal)
    {
      return (scaled / columns) * columns;
    }

    if (orientation == Vertical)
    {
      return (scaled / rows) * rows;
    }

    return (scaled / rows) * rows;
  }

  bool Grid::setup()
  {
    if (length == 0)
    {
      return false;
    }

    if (rows == 0)
    {
      rows = 1;
    }

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
    return true;
  }

  bool Grid::setup(uint16_t p_length, uint16_t p_rows, uint8_t p_origin, uint8_t p_orientation)
  {
    length = p_length;
    rows = p_rows;
    origin = p_origin;
    orientation = p_orientation;
    return setup();
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

} // namespace glow
