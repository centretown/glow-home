#pragma once

#include <utility>

#include "esphome.h"
using namespace esphome;
using namespace light;

#include "Lamp.h"

namespace glow
{
  class RowsGradient : public Lamp
  {
  public:
    void apply()
    {
      if (!is_ready())
        return;

      uint8_t amnt = 255 / length;
      auto color_mapper = [&](uint16_t i)
      {
        return chroma.step_gradient(amnt * i);
      };
      spin(0, length, color_mapper);
    }
  };

  class RowsFlatGradient : public Lamp
  {
  public:
    void apply()
    {
      if (!is_ready())
        return;

      uint8_t amnt = 255 / grid.Rows();
      auto color_mapper = [&](uint16_t i)
      {
        return chroma.step_gradient(amnt * (i / grid.Columns()));
      };
      spin(0, length, color_mapper);
    }
  };

  class ColumnsGradient : public Lamp
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

      auto color_mapper = [&](uint16_t i)
      {
        return chroma.step_gradient(amnt * i);
      };

      spin(0, length, mapper, color_mapper);
    }
  };

  class ColumnsFlatGradient : public Lamp
  {
  public:
    void apply()
    {
      if (!is_ready())
        return;

      uint8_t amnt = 255 / grid.Columns();
      div_t point{0, 0};

      auto mapper = [&](uint16_t i)
      {
        return map_columns(i, point);
      };

      auto color_mapper = [&](uint16_t i)
      {
        return chroma.step_gradient(amnt * point.quot);
      };

      spin(0, length, mapper, color_mapper);
    }
  };

  class DiagonalGradient : public Lamp
  {
  public:
    void apply()
    {
      if (!is_ready())
        return;

      // uint16_t count = grid.Rows() + grid.Columns() - 1;
      uint8_t amt = 255 / length;

      auto mapper = [&](uint16_t i)
      {
        return map_diagonal(i);
      };

      auto color_mapper = [&](uint16_t i)
      {
        return chroma.step_gradient(i * amt);
      };

      spin(0, length, mapper, color_mapper);
    }
  };
}
