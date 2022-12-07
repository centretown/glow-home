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

      if (next - now > interval && is_setup == 1)
      {
        next = now + interval;
        return true;
      }
      return false;
    }

    void update_hue() ALWAYS_INLINE
    {
      if (delta != 0)
      {
        hsv_color.hue += delta;
        color = hsv_color.to_rgb();
      }
    }

    bool check_setup() ALWAYS_INLINE
    {
      if (!is_setup)
      {
        log_not_setup();
        return false;
      }

      return true;
    }

    virtual void log() const
    {
      ESP_LOGD("glow-Base",
               "rows=%u, columns=%u, interval=%u, delta=%d",
               rows, columns, interval, delta);
      ESP_LOGD("glow-Base",
               "color=red:%u green:%u blue:%u",
               color.red, color.green, color.blue);
      ESP_LOGD("glow-Base",
               "hsv_color=hue:%u saturation:%u value:%u",
               hsv_color.hue, hsv_color.saturation,
               hsv_color.value);
      ESP_LOGD("glow-Base",
               "is_setup=%u, is_logged=%u",
               is_setup, is_logged);
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
  };

}
