
#include "Gradients.h"

namespace glow
{
  void Gradients::as_columns()
  {
    if (!check_setup())
      return;

    uint8_t amnt = 255 / length;
    for (int32_t i = 0; i < length; i++)
    {
      div_t point = div(i, rows);
      uint16_t mapped = (uint16_t)(point.rem * columns + point.quot);
      light->get(mapped) = step_gradient(amnt * i);
    }
  }

  void Gradients::as_columns_flat()
  {
    if (!check_setup())
      return;

    uint8_t amnt = 255 / columns;
    for (int32_t i = 0; i < length; i++)
    {
      div_t point = div(i, rows);
      uint16_t mapped = (uint16_t)(point.rem * columns + point.quot);
      light->get(mapped) = step_gradient(amnt * point.quot);
    }
  }

  void Gradients::as_rows()
  {
    if (!check_setup())
      return;

    uint8_t amnt = 255 / length;
    for (int32_t i = 0; i < length; i++)
    {
      light->get(i) = step_gradient(amnt * i);
    }
  }

  void Gradients::as_rows_flat()
  {
    if (!check_setup())
      return;

    uint8_t amnt = 255 / rows;
    for (int32_t i = 0; i < length; i++)
    {
      light->get(i) = step_gradient(amnt * (i / columns));
    }
  }

  void Gradients::as_diagonal()
  {
    if (!check_setup())
      return;

    uint8_t amnt = 255 / (columns + rows - 1);
    uint8_t sat = 0;
    
    for (int16_t c = -rows + 1; c < columns; c++, sat++)
    {
      for (int16_t r = 0; r < rows; r++)
      {
        if (c + r < 0)
        {
          continue;
        }
        if (c + r >= columns)
        {
          break;
        }

        light->get(r * columns + c + r) = step_gradient(amnt * sat);
      }
    }
  }

}