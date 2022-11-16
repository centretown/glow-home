#pragma once

#include "esphome.h"
using namespace esphome;
using namespace light;

namespace glow
{
  class Pixels
  {
  private:
    AddressableLight *light = nullptr;
    int32_t length = 0;
    uint16_t rows = 1;
    uint16_t columns = 1;
    bool is_setup = false;
    bool is_logged = false;

  public:
    inline void setup(AddressableLight *it,
                      uint16_t row_count = 1)
    {
      light = it;
      length = light->size();
      set_rows(row_count);
    }

    void set_rows(uint16_t row_count)
    {
      if (row_count < length)
      {
        rows = row_count;
        columns = length / rows;
      }
    }

    uint16_t size()
    {
      return length;
    }

    inline void Put(uint16_t index, ESPHSVColor value)
    {
      light->get(index) = value;
    }

    inline void Put(uint16_t index, Color value)
    {
      light->get(index) = value;
    }

    inline void Update() {}

    void gradient(Color from, Color to)
    {
      uint8_t amnt = 255 / length;
      Color color;

      auto map = [&](uint16_t index)
      {
        if (rows < 2)
        {
          return index;
        }
        div_t point = div(index, rows);
        return (uint16_t)(point.rem * columns + point.quot);
      };

      for (int32_t i = 0; i < length; i++)
      {
        color = from.gradient(to, amnt * i);
        light->get(map(i)) = color;
      }
    }

    void log_setup()
    {
      is_logged = true;
    }

    void log_not_setup()
    {
      if (!is_logged)
      {
        ESP_LOGD("glow-Pixels", "Not Setup!");
        is_logged = true;
      }
    }
  };

  ESPHSVColor hsv_color(Color color);
}
