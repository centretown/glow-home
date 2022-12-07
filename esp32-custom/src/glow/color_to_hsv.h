#pragma once

#include "esphome.h"
using namespace esphome;
using namespace light;

namespace glow
{
  static ESPHSVColor color_to_hsv(Color color)
  {
    float red = static_cast<float>(color.red) / 255.0;
    float green = static_cast<float>(color.green) / 255.0;
    float blue = static_cast<float>(color.blue) / 255.0;
    float saturation, value;
    int hue;
    rgb_to_hsv(red, green, blue, hue, saturation, value);
    hue *= 255;
    hue /= 360;
    saturation *= 255;
    value *= 255;
    return ESPHSVColor(static_cast<uint8_t>(hue),
                       static_cast<uint8_t>(saturation),
                       static_cast<uint8_t>(value));
  }
}