#pragma once

#include <utility>

#include "esphome.h"
using namespace esphome;
using namespace light;

#include "Gradient.h"

namespace glow
{
  class ColumnsGradient : public Gradient
  {
  public:
    void apply()
    {
      if (!is_ready())
        return;

      uint8_t amnt = 255 / length;
      for (int32_t i = 0; i < length; i++)
      {
        div_t point = div(i, rows);
        uint16_t mapped = (uint16_t)(point.rem * columns + point.quot);
        light->get(mapped) = step_gradient(amnt * i);
      }
      update_hue();
    }
  };

  class ColumnsFlatGradient : public Gradient
  {
  public:
    void apply()
    {
      if (!is_ready())
        return;

      uint8_t amnt = 255 / columns;
      for (int32_t i = 0; i < length; i++)
      {
        div_t point = div(i, rows);
        uint16_t mapped = (uint16_t)(point.rem * columns + point.quot);
        light->get(mapped) = step_gradient(amnt * point.quot);
      }
      update_hue();
    }
  };

  class RowsGradient : public Gradient
  {
  public:
    void apply()
    {
      if (!is_ready())
        return;
        
      uint8_t amnt = 255 / length;
      for (int32_t i = 0; i < length; i++)
      {
        light->get(i) = step_gradient(amnt * i);
      }
      update_hue();
    }
  };

  class RowsFlatGradient : public Gradient
  {
  public:
    void apply()
    {
      if (!is_ready())
        return;
      uint8_t amnt = 255 / rows;
      for (int32_t i = 0; i < length; i++)
      {
        light->get(i) = step_gradient(amnt * (i / columns));
      }
      update_hue();
    }
  };

  class DiagonalGradient : public Gradient
  {
  public:
    void apply()
    {
      if (!is_ready())
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
      update_hue();
    }
  };
}
