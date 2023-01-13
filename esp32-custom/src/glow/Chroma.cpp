#include "Chroma.h"

namespace glow
{
  ESPHSVColor Chroma::color_to_hsv(Color color)
  {
    float red = static_cast<float>(color.red) / 255.0;
    float green = static_cast<float>(color.green) / 255.0;
    float blue = static_cast<float>(color.blue) / 255.0;
    float saturation, value;
    int hue;

#ifndef USE_ESP32
    rgb_to_hsv(red, green, blue, hue, saturation, value);
#else
    esphome::rgb_to_hsv(red, green, blue, hue, saturation, value);
#endif

    hue *= 255;
    hue /= 360;
    saturation *= 255;
    value *= 255;
    return ESPHSVColor(static_cast<uint8_t>(hue),
                       static_cast<uint8_t>(saturation),
                       static_cast<uint8_t>(value));
  }

  void Chroma::setup(Color current_color, ESPHSVColor hsv, int16_t delta)
  {
    rgb_source = current_color;
    hsv_source = color_to_hsv(rgb_source);
    hsv_target = hsv;
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
}