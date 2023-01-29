#include "HSVColor.h"

namespace glow
{
  uint32_t hsv_to_u32(ESPHSVColor hsv)
  {
    return HSVColor(hsv).raw_32;
  }

  ESPHSVColor u32_to_hsv(uint32_t hsv32)
  {
    return HSVColor(hsv32).esp_hsv();
  }

  ESPHSVColor color_to_hsv(Color color)
  {
    const uint16_t red = color.red;
    const uint16_t green = color.green;
    const uint16_t blue = color.blue;

    const uint16_t value = std::max(red, std::max(green, blue));
    const uint16_t color_range = value -
                                 std::min(red, std::min(green, blue));
    const uint16_t saturation =
        (value == 0) ? 0
                     : (color_range * byte_limit) / value;
    uint16_t hue = 0;
    if (color_range != 0)
    {
      if (value == red)
      {
        hue = (hue_segment * (green - blue) / color_range) + hue_limit;
      }
      else if (value == green)
      {
        hue = (hue_segment * (blue - red) / color_range) + hue_green;
      }
      else // if (value == blue)
      {
        hue = (hue_segment * (red - green) / color_range) + hue_blue;
      }
    }

    return ESPHSVColor(static_cast<uint8_t>(hue / 6),
                       static_cast<uint8_t>(saturation),
                       static_cast<uint8_t>(value));
  }

#ifndef USE_ESP32
  // keep original for testing
  ESPHSVColor old_color_to_hsv(Color color)
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
#endif

} // namespace name
