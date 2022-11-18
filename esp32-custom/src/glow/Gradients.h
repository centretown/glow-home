#pragma once

#include "esphome.h"
using namespace esphome;
using namespace light;

namespace glow
{
  class Gradients
  {
  private:
    AddressableLight *light = nullptr;
    uint16_t length = 0;
    uint16_t rows = 1;
    uint16_t columns = 1;
    Color base_color;
    uint32_t next = 0;
    uint32_t interval = 48;

    union
    {
      struct
      {
        uint is_setup : 1;
        uint is_logged : 1;
        uint reserved : 14;
      };
      uint16_t flags = 0;
    };

  public:
    void setup(AddressableLight *it,
               uint16_t row_count = 1)
    {
      light = it;
      length = (uint16_t)light->size();
      set_rows(row_count);
      is_setup = 1;
      log_setup();
    }

    void set_rows(uint16_t row_count)
    {
      if (row_count < length)
      {
        rows = row_count;
        columns = length / rows;
      }
    }

    inline void set_interval(uint32_t v)
    {
      interval = v;
    }

    inline void set_base_color(ESPHSVColor hsv)
    {
      base_color = hsv.to_rgb();
    }

    inline void set_color(Color c)
    {
      base_color = c;
    }

    void columns_to(Color to)
    {
      if (!check_setup())
        return;

      uint8_t amnt = 255 / length;
      Color color;
      for (int32_t i = 0; i < length; i++)
      {
        div_t point = div(i, rows);
        uint16_t j = (uint16_t)(point.rem * columns + point.quot);
        color = color_gradient(base_color, to, amnt * i);
        light->get(j) = color;
      }
    }

    void columns_paralell_to(Color to)
    {
      if (!check_setup())
        return;

      uint8_t amnt = 255 / columns;
      Color color;
      for (int32_t i = 0; i < length; i++)
      {
        div_t point = div(i, rows);
        uint16_t j = (uint16_t)(point.rem * columns + point.quot);
        color = color_gradient(base_color, to, amnt * point.quot);
        light->get(j) = color;
      }
    }

    void rows_to(Color to)
    {
      if (!check_setup())
        return;

      uint8_t amnt = 255 / length;
      Color color;
      for (int32_t i = 0; i < length; i++)
      {
        color = color_gradient(base_color, to, amnt * i);
        light->get(i) = color;
      }
    }

    void rows_paralell_to(Color to)
    {
      if (!check_setup())
        return;

      uint8_t amnt = 255 / rows;
      Color color;
      for (int32_t i = 0; i < length; i++)
      {
        color = color_gradient(base_color, to, amnt * (i / columns));
        light->get(i) = color;
      }
    }

    inline bool check_setup()
    {
      if (!is_setup)
      {
        log_not_setup();
        return false;
      }

      return true;
    }

    void log_setup()
    {
      if (is_logged)
      {
        return;
      }
      log_data();
      log_flags();
      is_logged = 1;
    }

    void log_not_setup()
    {
      if (is_logged)
      {
        return;
      }
      ESP_LOGD("glow-Pixels", "Not Setup!");
      is_logged = 1;
    }

    void log_data()
    {
      ESP_LOGD("glow-Pixels",
               "length=%d, rows=%u, columns=%u",
               length, rows, columns);
    }

    void log_flags()
    {
      ESP_LOGD("glow-Pixels",
               "is_setup=%u, is_logged=%u",
               is_setup, is_logged);
    }

    inline bool check_interval(uint32_t now)
    {
      if (next - now > interval)
      {
        next = now + interval;
        return true;
      }

      return false;
    }

  public:
    static ESPHSVColor hsv_color(Color color);
    static Color color_gradient(const Color &from, const Color &to, uint8_t amnt);
  };

}
