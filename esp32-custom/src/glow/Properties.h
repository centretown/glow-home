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
  struct GlowHSVColor
  {
    union
    {
      struct
      {
        uint8_t h;
        uint8_t s;
        uint8_t v;
        uint8_t u;
      };
      uint32_t raw_32 = 0;
    };
    GlowHSVColor(uint32_t raw_32) ALWAYS_INLINE : raw_32(raw_32) {}
    GlowHSVColor(uint8_t h = 0, uint8_t s = 0, uint8_t v = 0) ALWAYS_INLINE : h(h), s(s), v(v) {}
    GlowHSVColor(ESPHSVColor hsv) ALWAYS_INLINE : h(hsv.h), s(hsv.s), v(hsv.v), u(0) {}
    ESPHSVColor esp_hsv() ALWAYS_INLINE { return ESPHSVColor(h, s, v); }
  };

  struct Properties
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
      INTERVAL,
      SCAN_WIDTH,
      ORIGIN,
      ORIENTATION,
      LENGTH,
      ROWS,
      SOURCE,
      TARGET,
      SHIFT,
      TRANSFORM,
      PROPERTY_COUNT,
    };

    uint32_t interval = 48;
    uint16_t scan_width = 0;
    uint8_t origin = TopLeft;
    uint8_t orientation = Horizontal;
    uint16_t length = 36;
    uint16_t rows = 4;
    ESPHSVColor source{0, 255, 255};
    ESPHSVColor target{127, 0, 255};
    int16_t shift = -1;
    uint8_t transform = AsIs;

    void set_grid(uint16_t len, uint16_t row_count, uint8_t org, uint8_t orient)
    {
      length = len;
      rows = row_count;
      origin = org;
      orientation = orient;
    }

    void set_chroma(Color s, Color t) ALWAYS_INLINE
    {
      source = color_to_hsv(s);
      target = color_to_hsv(t);
    }

    void set_chroma(ESPHSVColor s, ESPHSVColor t) ALWAYS_INLINE
    {
      source = s;
      target = t;
    }

    void set_chroma(Color rgb, ESPHSVColor t) ALWAYS_INLINE
    {
      source = color_to_hsv(rgb);
      target = t;
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
    bool set(const char *key, char *value);
    void set(uint8_t key, char *value);
    bool get(const char *key, char *value, size_t value_length);
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

    static uint32_t hsv_to_u32(ESPHSVColor hsv) ALWAYS_INLINE
    {
      return GlowHSVColor(hsv).raw_32;
    }

    static ESPHSVColor u32_to_hsv(uint32_t hsv32) ALWAYS_INLINE
    {
      return GlowHSVColor(hsv32).esp_hsv();
    }

#ifndef USE_ESP32
    // keep original for testing and benchmarking
    static ESPHSVColor old_color_to_hsv(Color color);
#endif
    static ESPHSVColor color_to_hsv(Color color);
  };
}