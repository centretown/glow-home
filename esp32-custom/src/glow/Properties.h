#pragma once
#include <unordered_map>
#include <array>
#include <string>

#include "base.h"

#include "../esphome/core/color.h"
#include "../esphome/components/light/esp_hsv_color.h"
using esphome::Color;
using esphome::light::ESPHSVColor;

#include "HSVColor.h"
#include "ExchangeItem.h"

namespace glow
{
  struct Properties
  {
#ifndef USE_ESP32
  private:
    static ExchangeItem<Properties> exchange[];
#endif

  public:
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

#ifndef USE_ESP32
    enum : uint8_t
    {
      INTERVAL,
      SCAN,
      ORIGIN,
      ORIENTATION,
      LENGTH,
      ROWS,
      BEGIN,
      END,
      SOURCE,
      TARGET,
      SHIFT,
      TRANSFORM,
      PROPERTY_COUNT,
    };

#endif

    uint32_t interval = 48;
    uint16_t scan = 0;
    uint8_t origin = TopLeft;
    uint8_t orientation = Horizontal;
    uint16_t length = 36;
    uint16_t rows = 4;
    uint16_t begin = 0;
    uint16_t end = 36;
    ESPHSVColor source{0, 255, 255};
    ESPHSVColor target{127, 0, 255};
    int16_t shift = -1;
    uint8_t transform = AsIs;

  public:
    Properties &copy(Properties &p);

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

#ifndef USE_ESP32
    // generic property interface
    bool set(const char *key, char *value);
    bool get(const char *key, char *value, size_t value_length);
    inline static uint8_t count()
    {
      return PROPERTY_COUNT;
    }
    static std::unordered_map<std::string, uint8_t> property_map;
    static ExchangeItem<Properties> &exchange_item(uint8_t id)
    {
      return exchange[id % count()];
    }
#endif
  };

}