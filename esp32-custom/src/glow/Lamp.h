#pragma once

#include "esphome.h"
using namespace esphome;
using namespace light;

#include "Presets.h"
#include "color_to_hsv.h"

namespace glow
{
  class Lamp
  {
  protected:
    AddressableLight *light = nullptr;
    uint16_t length = 0;
    uint32_t interval = 48;
    int16_t delta = 1;

    Color color;
    ESPHSVColor hsv_color;

    uint16_t rows = 1;
    uint16_t columns = 0;
    uint32_t next = 0;

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
    virtual void setup(AddressableLight *it, Color current_color)
    {
      light = it;
      if (light == nullptr)
      {
        log_not_setup();
        return;
      }

      length = (uint16_t)light->size();
      color = current_color;
      hsv_color = color_to_hsv(color);

      Presets presets;
      presets.setup();
      set_interval(presets.interval);
      set_delta(presets.delta);
      set_rows(presets.rows);

      is_setup = 1;
      log();
    }

    void set_rows(uint16_t row_count)
    {
      if (row_count < length)
      {
        rows = row_count;
        columns = length / rows;
      }
    }

    void set_interval(int16_t v) ALWAYS_INLINE
    {
      interval = v;
    }

    void set_delta(int16_t d) ALWAYS_INLINE
    {
      delta = d;
    }

    bool is_ready() ALWAYS_INLINE
    {
#define millis() esphome::millis()
      const uint32_t now = millis();

      if (next - now > interval && is_setup)
      {
        next = now + interval;
        return true;
      }
      return false;
    }

    void update_hue() ALWAYS_INLINE
    {
      if (delta == 0)
        return;

      color = update_hue(hsv_color, delta);
    }

    Color update_hue(ESPHSVColor &hsv, int16_t change) ALWAYS_INLINE
    {
      hsv.hue += change;
      return hsv.to_rgb();
    }

    virtual void log() const
    {
      ESP_LOGD("glow-Lamp",
               "rows=%u, columns=%u, interval=%u, delta=%d",
               rows, columns, interval, delta);
      ESP_LOGD("glow-Lamp",
               "color=red:%u green:%u blue:%u",
               color.red, color.green, color.blue);
      ESP_LOGD("glow-Lamp",
               "hsv_color=hue:%u saturation:%u value:%u",
               hsv_color.hue, hsv_color.saturation,
               hsv_color.value);
      ESP_LOGD("glow-Lamp",
               "is_setup=%u, is_logged=%u",
               is_setup, is_logged);
    }

    void log_not_setup()
    {
      if (is_logged)
        return;
      ESP_LOGD("glow-Pixels", "Not Setup!");
      is_logged = 1;
    }

    uint16_t map_columns(uint16_t i, div_t &point) ALWAYS_INLINE
    {
      point = div(i, rows);
      return (uint16_t)(point.rem * columns + point.quot);
    }

    uint16_t map_diagonal(uint16_t i, div_t &point) ALWAYS_INLINE
    {
      point = div(i, rows);
      //    rem,quot; (rows=4)
      // 0 = 0,0
      // 1 = 1,0
      // 2 = 2,0
      // 3 = 3,0
      // 4 = 0,1
    }

    template <typename CHROMA>
    void spin(uint16_t begin, uint16_t end,
              CHROMA &chroma)
    {
      for (uint16_t i = begin; i < end; ++i)
      {
        light->get(i) = chroma(i);
      }
    }

    template <typename MAPPER, typename CHROMA>
    void spin(uint16_t begin, uint16_t end,
              MAPPER &mapper, CHROMA &chroma)
    {
      for (uint16_t i = begin; i < end; ++i)
      {
        uint16_t offset = mapper(i);
        light->get(offset) = chroma(i);
      }
      update_hue();
    }
  };

}
