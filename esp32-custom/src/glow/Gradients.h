#pragma once

#include "esphome.h"
using namespace esphome;
using namespace light;

#include "Presets.h"

namespace glow
{
  class Gradients
  {
  private:
    AddressableLight *light = nullptr;
    uint16_t length = 0;
    uint16_t columns = 1;
    uint32_t next = 0;
    Color begin;
    Color end;

    // from presets
    ESPHSVColor hsv_begin;
    ESPHSVColor hsv_end;
    uint16_t rows = 4;
    uint32_t interval = 48;
    int16_t delta = 1;

    union
    {
      struct
      {
        uint is_setup : 1;
        uint is_logged : 1;
        uint reserved : 6;
      };
      uint8_t flags = 0;
    };

  public:
  
    void setup(AddressableLight *it, Color current_color)
    {
      if (setup_light(it) == false)
        return;

      Presets presets;
      presets.setup(current_color);
      setup_from_presets(presets);
    }

    void setup(AddressableLight *it, const Presets &presets)
    {
      if (setup_light(it) == false)
        return;
      setup_from_presets(presets);
    }

    bool setup_light(AddressableLight *it)
    {
      light = it;
      if (light == nullptr)
      {
        log_not_setup();
        return false;
      }
      return true;
    }

    void setup_from_presets(const Presets &presets)
    {
      presets.log();
      hsv_begin = presets.begin;
      begin = hsv_begin.to_rgb();
      hsv_end = presets.end;
      end = hsv_end.to_rgb();

      length = (uint16_t)light->size();
      set_rows_and_columns(presets.rows);
      set_interval_and_delta(presets.interval, presets.delta);
      log_setup();
    }

    void set_rows_and_columns(uint16_t row_count)
    {
      if (row_count < length)
      {
        rows = row_count;
        columns = length / rows;
      }
    }

    inline void set_interval_and_delta(uint32_t v, int16_t d)
    {
      interval = v;
      delta = d;
    }

    bool check_interval(uint32_t now) ALWAYS_INLINE
    {
      if (next - now > interval)
      {
        if (delta != 0)
        {
          hsv_begin.hue += delta;
          begin = hsv_begin.to_rgb();
        }
        next = now + interval;
        return true;
      }

      return false;
    }

    // implement

    void as_columns();
    void as_columns_flat();
    void as_rows();
    void as_rows_flat();
    void as_diagonal();

    Color step_gradient(uint8_t amnt)
    {
      Color color;
      float amnt_f = float(amnt) / 255.0f;
      color.r = begin.r + amnt_f * (end.r - begin.r);
      color.g = begin.g + amnt_f * (end.g - begin.g);
      color.b = begin.b + amnt_f * (end.b - begin.b);
      return color;
    }

    // guard

    bool check_setup()
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
      log_data();
      log_colors();
      log_flags();
      is_logged = 1;
      is_setup = 1;
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
      ESP_LOGD("glow-Pixels",
               "interval=%u, delta=%d",
               interval, delta);
    }

    void log_colors()
    {
      ESP_LOGD("glow-Pixels",
               "begin=red:%u green:%u blue:%u",
               begin.red, begin.green, begin.blue);
      ESP_LOGD("glow-Pixels",
               "end=red:%u green:%u blue:%u",
               end.red, end.green, end.blue);
    }

    void log_flags()
    {
      ESP_LOGD("glow-Pixels",
               "is_setup=%u, is_logged=%u",
               is_setup, is_logged);
    }
  };

}
