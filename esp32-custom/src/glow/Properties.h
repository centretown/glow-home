#pragma once
#include <unordered_map>
#include <string>

#include "base.h"
#include "../esphome/core/color.h"
#include "../esphome/components/light/esp_hsv_color.h"

using esphome::Color;
using esphome::light::ESPHSVColor;

namespace glow
{
  enum : uint8_t
  {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
  };

  enum : uint8_t
  {
    Horizontal,
    Vertical,
    Diagonal
  };

  enum : uint8_t
  {
    ShiftNone = 0,
    ShiftHue = 1,
    ShiftSaturation = 2,
    ShiftValue = 4,
  };

  enum : uint8_t
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

  enum : uint8_t
  {
    CURRENT_COLOR,
    ORIGIN,
    ORIENTATION,
    LENGTH,
    COLOR_TRANSFORM,
    GRID_ROWS,
    HUE_DELTA,
    GRADIENT_HUE,
    GRADIENT_SATURATION,
    GRADIENT_VALUE,
    SCAN_WIDTH,
    PROPERTY_COUNT,
  };

  struct Properties
  {
    Color current_color{255, 0, 0};
    uint8_t origin = TopLeft;
    uint8_t orientation = Horizontal;
    uint16_t length = 36;
    uint8_t color_transform = AsIs;

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

    void operator()(uint16_t len, float rows, uint8_t org, uint8_t orient)
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

    bool get_key(uint8_t key, char *buffer, size_t buffer_length)
    {
      if (key >= PROPERTY_COUNT)
      {
        return false;
      }

      strncpy(buffer, property_names[key].c_str(), buffer_length);
      return true;
    }

    // generic property interface
    void set(const char *key, char *value);
    void set(uint8_t key, char *value);
    void get(const char *key, char *value, size_t value_length);
    void get(uint8_t key, char *value, size_t value_length);

    static bool check(const char *key)
    {
      if (property_map.find(key) == property_map.end())
      {
        return false;
      }
      return true;
    }

    static uint8_t count()
    {
      return PROPERTY_COUNT;
    }

    static std::string property_names[];
    static std::unordered_map<std::string, uint8_t> property_map;
  };
}