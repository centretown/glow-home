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
      div_t point{0, 0};

      auto mapper = [&](uint16_t i)
      {
        return map_columns(i, point);
      };

      auto chroma = [&](uint16_t i)
      {
        return step_gradient(amnt * i);
      };

      spin(0, length, mapper, chroma);
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
      div_t point{0, 0};

      auto mapper = [&](uint16_t i)
      {
        return map_columns(i, point);
      };

      auto chroma = [&](uint16_t i)
      {
        return step_gradient(amnt * point.quot);
      };

      spin(0, length, mapper, chroma);
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
      auto chroma = [&](uint16_t i)
      {
        return step_gradient(amnt * i);
      };
      spin(0, length, chroma);
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
      auto chroma = [&](uint16_t i)
      {
        return step_gradient(amnt * (i / columns));
      };
      spin(0, length, chroma);
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

      auto mapper = [&](uint16_t i)
      {
        // return map_columns(i);
      };

      auto chroma = [&](uint16_t i)
      {
        return step_gradient(amnt * i);
      };

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
