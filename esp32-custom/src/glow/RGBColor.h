#pragma once

#include <string>

#include "base.h"
#ifndef MICRO_CONTROLLER
#include <yaml-cpp/yaml.h>
#endif

#ifdef ESPHOME_CONTROLLER
#include "esphome.h"
#include "../esphome/core/color.h"
#endif

#include "stdint.h"

namespace glow
{
  struct Color
  {
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint8_t white = 0;

    Color() = default;

    Color(uint8_t p_red, uint8_t p_green, uint8_t p_blue) ALWAYS_INLINE
    {
      red = p_red;
      green = p_green;
      blue = p_blue;
      white = 0;
    }

    inline Color &operator=(const Color &color)
    {
      red = color.red;
      green = color.green;
      blue = color.blue;
      white = color.white;
      return *this;
    }

    inline bool operator==(const Color &color)
    {
      return (red == color.red &&
              green == color.green &&
              blue == color.blue);
    }

#ifdef ESPHOME_CONTROLLER
    esphome::Color get() ALWAYS_INLINE
    {
      return esphome::Color{red, green, blue};
    }
#else
    Color get() ALWAYS_INLINE
    {
      return Color{red, green, blue};
    }
#endif

#ifndef MICRO_CONTROLLER
    enum : uint8_t
    {
      RED,
      GREEN,
      BLUE,
      KEY_COUNT,
    };

    static std::string keys[KEY_COUNT];
#endif
  };

} // namespace glow

#ifndef MICRO_CONTROLLER
namespace YAML
{
  using glow::Color;
  template <>
  struct convert<Color>
  {
    static Node encode(const Color &color)
    {
      Node node;
      node[Color::keys[Color::RED]] = static_cast<uint16_t>(color.red);
      node[Color::keys[Color::GREEN]] = static_cast<uint16_t>(color.green);
      node[Color::keys[Color::BLUE]] = static_cast<uint16_t>(color.blue);
      return node;
    }

    static bool decode(const Node &node, Color &color)
    {
      if (!node.IsMap())
      {
        return false;
      }
      color.red = node[Color::keys[Color::RED]].as<uint8_t>();
      color.green = node[Color::keys[Color::GREEN]].as<uint8_t>();
      color.blue = node[Color::keys[Color::BLUE]].as<uint8_t>();
      return true;
    }
  };
}
#endif