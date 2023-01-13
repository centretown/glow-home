#pragma once

#include "base.h"

#include "../esphome/core/color.h"
#include "../esphome/components/light/esp_hsv_color.h"

using esphome::Color;
using esphome::light::ESPHSVColor;

namespace glow
{
  class Chroma
  {
    // private:
  public:
    Color rgb_source;
    ESPHSVColor hsv_source;

    Color rgb_target;
    ESPHSVColor hsv_target;

    int16_t delta = 1;

  public:
    void setup(Color current_color, ESPHSVColor hsv, int16_t delta);

    void update_hue() ALWAYS_INLINE
    {
      if (delta == 0)
        return;
      hsv_source.hue += delta;
      rgb_source = hsv_source.to_rgb();
    }

    Color step_gradient(uint8_t amnt) ALWAYS_INLINE
    {
      Color step;
      float amnt_f = static_cast<float>(amnt) / 255.0f;
      step.r = rgb_source.r + amnt_f * (rgb_target.r - rgb_source.r);
      step.g = rgb_source.g + amnt_f * (rgb_target.g - rgb_source.g);
      step.b = rgb_source.b + amnt_f * (rgb_target.b - rgb_source.b);
      return step;
    }

    static ESPHSVColor color_to_hsv(Color color);
    void log_buffer(char *buffer, size_t buffer_size) const;
  };

}
