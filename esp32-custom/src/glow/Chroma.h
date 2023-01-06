#pragma once

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
    Color target;
    ESPHSVColor hsv_target;

  public:
    void setup(uint8_t hue, uint8_t saturation, uint8_t value)
    {
      hsv_target.hue = hue;
      hsv_target.saturation = saturation;
      hsv_target.value = value;
      target = hsv_target.to_rgb();
    }
  };
}
