#pragma once

#include <math.h>
#include <algorithm>

#include "base.h"
#ifndef MICRO_CONTROLLER
#include <yaml-cpp/yaml.h>
#include <sstream>
#endif

#include "RGBColor.h"

namespace glow
{
  constexpr uint16_t byte_limit = 0xff;
  constexpr uint16_t hue_segment_count = 6;
  constexpr uint16_t hue_limit = byte_limit * hue_segment_count;
  constexpr uint16_t hue_segment = hue_limit / hue_segment_count;

  constexpr uint16_t hue_red = 0;
  constexpr uint16_t hue_yellow = hue_segment;
  constexpr uint16_t hue_green = hue_limit * 2 / hue_segment_count;
  constexpr uint16_t hue_cyan = hue_limit * 3 / hue_segment_count;
  constexpr uint16_t hue_blue = hue_limit * 4 / hue_segment_count;
  constexpr uint16_t hue_magenta = hue_limit * 5 / hue_segment_count;

  struct HSVColor
  {
    union
    {
      struct
      {
        uint16_t hue;
        uint8_t saturation;
        uint8_t value;
      };

      uint32_t raw_32 = 0;
    };

    HSVColor() = default;

    HSVColor(uint16_t hue, uint8_t saturation, uint8_t value)
        : hue(hue), saturation(saturation), value(value) {}

    bool setup()
    {
      return true;
    }

    Color to_rgb();

    void from_rgb(Color color);

    HSVColor to_gradient(HSVColor target, uint16_t index, uint16_t length) ALWAYS_INLINE
    {
      if (hue > target.hue)
      {
        target.hue += hue_limit;
      }
      uint16_t gradient_hue = hue + ((target.hue - hue) * index) / length;
      gradient_hue %= hue_limit;
      int16_t gradient_saturation = saturation + ((target.saturation - saturation) * index) / length;
      int16_t gradient_value = value + ((target.value - value) * index) / length;
      return HSVColor{static_cast<uint16_t>(gradient_hue),
                      static_cast<uint8_t>(gradient_saturation),
                      static_cast<uint8_t>(gradient_value)};
    }

    void to_color_wheel(float &f_hue, float &f_saturation, float &f_value) const
    {
      f_hue = round(static_cast<float>(hue) * 360.0f /
                    static_cast<float>(hue_limit));
      f_saturation = round(static_cast<float>(saturation) * 100.0f /
                           static_cast<float>(byte_limit));
      f_value = round(static_cast<float>(value) * 100.0f /
                      static_cast<float>(byte_limit));
    }

    void from_color_wheel(float f_hue, float f_saturation, float f_value)
    {
      f_hue = round(f_hue);
      f_saturation = round(f_saturation);
      f_value = round(f_value);

      hue = static_cast<uint16_t>(f_hue / 360.0f *
                                  static_cast<float>(hue_limit));
      saturation = static_cast<uint8_t>(f_saturation / 100.0f *
                                        static_cast<float>(byte_limit));
      value = static_cast<uint8_t>(f_value / 100.0f *
                                   static_cast<float>(byte_limit));
    }
#ifndef MICRO_CONTROLLER
    enum : uint8_t
    {
      HUE,
      SATURATION,
      VALUE,
      KEY_COUNT,
    };

    static std::string keys[KEY_COUNT];
    std::string make_code();
#endif
  };
}

#ifndef MICRO_CONTROLLER
namespace YAML
{
  using glow::HSVColor;

  template <>
  struct convert<HSVColor>
  {
    static Node encode(const HSVColor &hsv)
    {
      Node node;
      float f_hue, f_saturation, f_value;
      hsv.to_color_wheel(f_hue, f_saturation, f_value);
      node[HSVColor::keys[HSVColor::HUE]] = f_hue;
      node[HSVColor::keys[HSVColor::SATURATION]] = f_saturation;
      node[HSVColor::keys[HSVColor::VALUE]] = f_value;
      return node;
    }

    static bool decode(const Node &node, HSVColor &hsv)
    {
      if (!node.IsMap())
      {
        hsv.setup();
        return false;
      }

      float f_hue = 0.0;
      float f_saturation = 0.0;
      float f_value = 0.0;

      for (auto key = 0; key < HSVColor::KEY_COUNT; ++key)
      {
        Node item = node[HSVColor::keys[key]];
        if (!item.IsDefined())
        {
          continue;
        }

        switch (key)
        {
        case HSVColor::HUE:
          f_hue = item.as<float>();
          break;
        case HSVColor::SATURATION:
          f_saturation = item.as<float>();
          break;
        case HSVColor::VALUE:
          f_value = item.as<float>();
          break;
        }
      }

      hsv.from_color_wheel(f_hue, f_saturation, f_value);
      return true;
    }
  };
}
#endif