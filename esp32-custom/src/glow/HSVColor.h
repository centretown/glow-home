#pragma once

#include "base.h"
#include "../esphome/core/color.h"
#include "../esphome/components/light/esp_hsv_color.h"

using esphome::Color;
using esphome::light::ESPHSVColor;

namespace glow
{
  constexpr uint16_t byte_limit = 0xff;
  constexpr uint16_t hue_segment = byte_limit;
  constexpr uint16_t hue_limit = byte_limit * 6;

  constexpr uint16_t hue_red = 0;
  // constexpr uint16_t hue_yellow = hue_segment;
  constexpr uint16_t hue_green = hue_limit / 3;
  // constexpr uint16_t hue_cyan = hue_limit / 2;
  constexpr uint16_t hue_blue = hue_limit * 2 / 3;
  // constexpr uint16_t hue_magenta = hue_limit * 5 / 6;

  struct HSVColor
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
      uint32_t raw_32;
    };
    HSVColor(uint32_t raw_32 = 0) ALWAYS_INLINE : raw_32(raw_32) {}
    HSVColor(ESPHSVColor hsv) ALWAYS_INLINE : h(hsv.h), s(hsv.s), v(hsv.v), u(0) {}
    ESPHSVColor esp_hsv() ALWAYS_INLINE { return ESPHSVColor(h, s, v); }
  };

#ifndef USE_ESP32
  // keep original for testing and benchmarking
  ESPHSVColor old_color_to_hsv(Color color);
#endif
  ESPHSVColor color_to_hsv(Color color);

  uint32_t hsv_to_u32(ESPHSVColor hsv);
  ESPHSVColor u32_to_hsv(uint32_t hsv32);
}
