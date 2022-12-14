#pragma once

#include <utility>

#include "esphome.h"
using namespace esphome;
using namespace light;

#include "Gradient.h"

namespace glow
{
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

      uint8_t amnt = 255 / grid.Rows();
      auto chroma = [&](uint16_t i)
      {
        return step_gradient(amnt * (i / grid.Columns()));
      };
      spin(0, length, chroma);
    }
  };

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

      uint8_t amnt = 255 / grid.Columns();
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

  class DiagonalGradient : public Gradient
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

      auto chroma = [&](uint16_t i)
      {
        return step_gradient(i * amt);
      };

      spin(0, length, mapper, chroma);
    }
  };
}
