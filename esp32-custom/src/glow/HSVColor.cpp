#include "HSVColor.h"

namespace glow
{
#ifndef MICRO_CONTROLLER
  std::string HSVColor::keys[HSVColor::KEY_COUNT] = {
      "hue",
      "saturation",
      "value",
  };

  std::string HSVColor::make_code()
  {
    std::stringstream s;
    s << "{" << hue << ","
      << (uint16_t)saturation << ","
      << (uint16_t)value << "}";
    return s.str();
  }
#endif

  Color HSVColor::to_rgb()
  {
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

    if (hue < hue_green)
    {
      if (hue < hue_yellow)
      {
        red = byte_limit;
        green = hue;
      }
      else // yellow to green
      {
        red = hue_green - hue;
        green = byte_limit;
      }
    }
    else if (hue < hue_blue)
    {
      if (hue < hue_cyan)
      {
        green = byte_limit;
        blue = hue - hue_green;
      }
      else // cyan to blue
      {
        green = hue_blue - hue;
        blue = byte_limit;
      }
    }
    else if (hue < hue_limit)
    {
      if (hue < hue_magenta)
      {
        blue = byte_limit;
        red = hue - hue_blue;
      }
      else // magenta to red
      {
        blue = hue_limit - hue;
        red = byte_limit;
      }
    }
    else
    {
      red = byte_limit;
    }

    const uint16_t saturation_multiplier = 1 + saturation;
    const uint16_t saturation_added = hue_segment - saturation;
    const uint16_t value_multiplier = 1 + value;

    Color rgbw;
    uint16_t color_result = (red * saturation_multiplier) >> 8;
    color_result += saturation_added;
    rgbw.red = (color_result * value_multiplier) >> 8;

    color_result = (green * saturation_multiplier) >> 8;
    color_result += saturation_added;
    rgbw.green = (color_result * value_multiplier) >> 8;

    color_result = (blue * saturation_multiplier) >> 8;
    color_result += saturation_added;
    rgbw.blue = (color_result * value_multiplier) >> 8;
    return rgbw;
  }

  void HSVColor::from_rgb(Color color)
  {
    const uint8_t red = color.red;
    const uint8_t green = color.green;
    const uint8_t blue = color.blue;

    const uint8_t primary = std::max({red, green, blue});
    const uint8_t color_range = primary -
                                std::min({red, green, blue});
    hue = 0;
    if (color_range != 0)
    {
      if (primary == red)
      {
        hue = (hue_segment * (green - blue) / color_range) + hue_limit;
      }
      else if (primary == green)
      {
        hue = (hue_segment * (blue - red) / color_range) + hue_green;
      }
      else // if (primary == blue)
      {
        hue = (hue_segment * (red - green) / color_range) + hue_blue;
      }
      hue %= hue_limit;
    }

    uint16_t sat = (primary == 0) ? 0 : (color_range * byte_limit) / primary;
    saturation = static_cast<uint8_t>(sat);
    value = static_cast<uint8_t>(primary);
  }

} // namespace glow
