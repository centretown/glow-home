#pragma once
#include "base.h"
#include "../esphome/core/color.h"
#include "../esphome/components/light/esp_hsv_color.h"

using esphome::Color;
using esphome::light::ESPHSVColor;

namespace glow
{
  enum Origin : uint8_t
  {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
  };

  enum Orientation : uint8_t
  {
    Horizontal,
    Vertical,
    Diagonal
  };

  enum ColorShift : uint8_t
  {
    ShiftNone = 0,
    ShiftHue = 1,
    ShiftSaturation = 2,
    ShiftValue = 4,
  };

  enum ColorTransform : uint8_t
  {
    AsIs,          // ff0000 -> ff0000
    Complementary, // ff0000 -> 00ffff (hue+128%255)
    Monochromatic, // ff0000 -> ff3333 (sat gradient)
    AnalogousA,    // ff0000 -> ff8000 (hue+)
    AnalogousB,    // ff0000 -> ff007f (hue+)
    TriadicA,      // ff0000 -> 00ff00
    TriadicB,      // ff0000 -> 0000ff
    TetradicA,     // ff0000 -> 7fff00
    TetradicB,     // ff0000 -> 00ffff
    TetradicC,     // ff0000 -> 8000ff
  };

  struct Properties
  {
    Color current_color;
    Origin origin = TopLeft;
    Orientation orientation = Horizontal;
    uint16_t length = 0;
    ColorTransform color_transform = AsIs;

    float update_interval = 48.0;
    float grid_rows = 4.0;
    float hue_delta = -1.0;

    float gradient_hue = 0.0;
    float gradient_saturation = 255.0;
    float gradient_value = 255.0;

    float scan_hue = 85.0;
    float scan_saturation = 255.0;
    float scan_value = 255.0;
    float scan_width = 1.0;

    void operator()(uint16_t len, float rows, Origin org, Orientation orient)
    {
      length = len;
      grid_rows = rows;
      origin = org;
      orientation = orient;
    }

    void operator()(Color rgb, float hue, float saturation, float value, float delta)
    {
      current_color = rgb;
      gradient_hue = hue;
      gradient_saturation = saturation;
      gradient_value = value;
    }
  };
}