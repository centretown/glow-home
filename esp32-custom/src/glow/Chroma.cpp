#include "Chroma.h"

namespace glow
{
  ESPHSVColor Chroma::color_to_hsv(Color color)
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

    return ESPHSVColor(static_cast<uint8_t>(hue/6),
                       static_cast<uint8_t>(saturation),
                       static_cast<uint8_t>(value));
  }

#ifndef USE_ESP32
  // keep original for testing
  ESPHSVColor Chroma::old_color_to_hsv(Color color)
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

  void Chroma::setup(Properties &properties)
  {
    rgb_source = static_cast<Color>(properties.current_color);
    hsv_source = color_to_hsv(rgb_source);
    hsv_target = ESPHSVColor(static_cast<uint8_t>(properties.gradient_hue),
                             static_cast<uint8_t>(properties.gradient_saturation),
                             static_cast<uint8_t>(properties.gradient_value));
    rgb_target = hsv_target.to_rgb();
  }

  void Chroma::log_buffer(char *buffer, size_t buffer_size) const
  {
    snprintf(buffer, buffer_size,
             "rgb source=r%u g%u b%u target=r%u g%u b%u\nhsv source=h%u s%u v%u target=h%u s%u v%u\n",
             rgb_source.red, rgb_source.green, rgb_source.blue,
             rgb_target.red, rgb_target.green, rgb_target.blue,
             hsv_source.hue, hsv_source.saturation, hsv_source.value,
             hsv_target.hue, hsv_target.saturation, hsv_target.value);
  }

  char *Chroma::log_hsv(char *buffer, size_t buffer_size, ESPHSVColor hsv)
  {
    snprintf(buffer, buffer_size,
             "h%u s%u v%u\n",
             hsv.hue, hsv.saturation, hsv.value);
    return buffer;
  }

  char *Chroma::log_rgb(char *buffer, size_t buffer_size, Color rgb)
  {
    snprintf(buffer, buffer_size,
             "r%u g%u b%u\n",
             rgb.red, rgb.green, rgb.blue);
    return buffer;
  }

}