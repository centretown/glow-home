#pragma once

#include "esphome.h"
using namespace esphome;
using namespace light;

extern template_::TemplateNumber *gradient_hue;
extern template_::TemplateNumber *gradient_saturation;
extern template_::TemplateNumber *gradient_value;
extern template_::TemplateNumber *gradient_rows;
extern template_::TemplateNumber *gradient_interval;
extern template_::TemplateNumber *gradient_delta;

namespace glow
{
  struct Presets
  {
    ESPHSVColor begin;
    ESPHSVColor end;
    uint16_t rows = 4;
    uint32_t interval = 48;
    int16_t delta = 1;

    void setup(
        Color current_color,
        float hue,
        float saturation,
        float value,
        float f_interval,
        float f_delta,
        float f_rows)
    {
      begin = hsv_color(current_color);
      end = ESPHSVColor((uint8_t)hue, (uint8_t)saturation, (uint8_t)value);
      interval = (uint32_t)f_interval;
      delta = (int16_t)f_delta;
      rows = (uint16_t)f_rows;
    }

    void setup(Color current_color)
    {
      begin = hsv_color(current_color);
      if (gradient_hue == nullptr)
      {
        ESP_LOGD("glow-Presets",
                 "gradient_hue has not been initialized.");
        return;
      }

      end = ESPHSVColor((uint8_t)gradient_hue->state,
                        (uint8_t)gradient_saturation->state,
                        (uint8_t)gradient_value->state);
      interval = (uint32_t)gradient_interval->state;
      delta = (int16_t)gradient_delta->state;
      rows = (uint16_t)gradient_rows->state;
    }

    void log() const
    {
      ESP_LOGD("glow-Presets",
               "rows=%u, interval=%u, delta=%d",
               rows, interval, delta);
      ESP_LOGD("glow-Presets",
               "begin=hue:%u saturation:%u value:%u",
               begin.hue, begin.saturation, begin.value);
      ESP_LOGD("glow-Presets",
               "end=hue:%u saturation:%u value:%u",
               end.hue, end.saturation, end.value);
    }

    static ESPHSVColor hsv_color(Color color)
    {
      float red = (float)color.red / 255.0;
      float green = (float)color.green / 255.0;
      float blue = (float)color.blue / 255.0;
      float saturation, value;
      int hue;
      rgb_to_hsv(red, green, blue, hue, saturation, value);
      hue *= 255;
      hue /= 360;
      saturation *= 255;
      value *= 255;
      return ESPHSVColor((uint8_t)hue, (uint8_t)saturation, (uint8_t)value);
    }
  };

}
