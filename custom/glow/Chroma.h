#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include "base.h"
#ifndef MICRO_CONTROLLER
#include <yaml-cpp/yaml.h>
#include "Palette.h"
#include "Filer.h"
#endif

#include "HSVColor.h"

namespace glow
{
  const HSVColor color_default = {0, 0, 255};

  class Chroma
  {
  private:
    uint16_t length = 0;
    int16_t hue_shift = 0;
  public:
    std::vector<HSVColor> colors;
    uint16_t segment_size{0};
    Color quick_color{0, 0, 0};

    Chroma() = default;

    Chroma(uint16_t p_length,
           HSVColor p_source = color_default,
           HSVColor p_target = color_default,
           int16_t p_hue_shift = 0)
    {
      setup(p_length, p_source, p_target, p_hue_shift);
    }

    Chroma(uint16_t p_length,
           std::initializer_list<HSVColor> p_colors,
           int16_t p_hue_shift = 0)
    {
      setup(p_length, p_colors, p_hue_shift);
    }

    bool setup(uint16_t p_length,
               HSVColor p_source = color_default,
               HSVColor p_target = color_default,
               int16_t p_hue_shift = 0);

    bool setup(uint16_t p_length,
               Color p_source,
               HSVColor p_target = color_default,
               int16_t p_hue_shift = 0);

    bool setup(uint16_t p_length,
               std::initializer_list<HSVColor> p_colors,
               int16_t p_hue_shift = 0);

    bool setup();

    bool setup_length(uint16_t p_length, int16_t p_hue_shift = 0)
    {
      length = p_length;
      hue_shift = p_hue_shift;
      return setup();
    }

    uint16_t get_length() const ALWAYS_INLINE { return length; }
    int16_t get_hue_shift() const ALWAYS_INLINE { return hue_shift; }
    HSVColor get_hsv_source() const ALWAYS_INLINE { return colors[0]; }
    HSVColor get_hsv_target() const ALWAYS_INLINE
    {
      if (colors.size() > 1)
        return colors[1];
      return colors[0];
    }
    Color get_rgb_source() const ALWAYS_INLINE { return get_hsv_source().to_rgb(); }
    Color get_rgb_target() const ALWAYS_INLINE { return get_hsv_target().to_rgb(); }

    void set_hue_shift(int16_t a_hue_shift)
    {
      hue_shift = a_hue_shift;
    }

    Color map(uint16_t index);

    void update();

#ifndef MICRO_CONTROLLER
    enum : uint8_t
    {
      LENGTH,
      SOURCE,
      TARGET,
      HUE_SHIFT,
      COLORS,
      KEY_COUNT,
    };

    static std::string keys[KEY_COUNT];
    friend YAML::convert<Chroma>;
    static bool load_palette(std::string file_name)
    {
      return load_yaml(file_name, palette);
    }

    std::string make_code();

    // private:
    static Palette palette;
#endif
  };
}

#ifndef MICRO_CONTROLLER
namespace YAML
{
  using glow::Chroma;

  template <>
  struct convert<Chroma>
  {
    static void lookup(Node &item, HSVColor &color)
    {
      if (item.IsMap())
      {
        color = item.as<HSVColor>();
        return;
      }

      PaletteColor palette_color;
      std::string name = item.as<std::string>();
      if (Chroma::palette.find_color(name, palette_color))
      {
        color = palette_color.hsv;
      }
    }

    static void lookup_list(Node &item, std::vector<HSVColor> &colors)
    {
      if (item.IsSequence())
      {
        HSVColor color;
        for (auto color_node : item)
        {
          lookup(color_node, color);
          colors.push_back(color);
        }
      }
    }

    static Node encode(const Chroma &chroma)
    {
      Node node;
      node[Chroma::keys[Chroma::LENGTH]] = chroma.length;
      node[Chroma::keys[Chroma::HUE_SHIFT]] = chroma.hue_shift;
      Node list;
      for (auto color : chroma.colors)
      {
        list.push_back(color);
      }
      node[Chroma::keys[Chroma::COLORS]] = list;
      return node;
    }

    static bool decode(const Node &node, Chroma &chroma)
    {
      static glow::HSVColor color_hold{0, 0, 0};
      if (!node.IsMap())
      {
        chroma.setup();
        return false;
      }

      for (auto key = 0; key < Chroma::KEY_COUNT; ++key)
      {
        Node item = node[Chroma::keys[key]];
        if (!item.IsDefined())
        {
          continue;
        }

        switch (key)
        {
        case Chroma::LENGTH:
          chroma.length = item.as<uint16_t>();
          break;
        case Chroma::HUE_SHIFT:
          chroma.hue_shift = item.as<int16_t>();
          break;
        case Chroma::SOURCE:
          lookup(item, color_hold);
          chroma.colors.push_back(color_hold);
          break;
        case Chroma::TARGET:
          lookup(item, color_hold);
          chroma.colors.push_back(color_hold);
          break;
        case Chroma::COLORS:
          lookup_list(item, chroma.colors);
          break;
        }
      }
      chroma.setup();
      return true;
    }
  };
}
#endif