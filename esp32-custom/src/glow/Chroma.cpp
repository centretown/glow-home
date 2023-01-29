#include "Chroma.h"

namespace glow
{
  void Chroma::setup(Properties &properties)
  {
    delta = static_cast<int16_t>(properties.shift);
    hsv_source = properties.source;
    rgb_source = hsv_source.to_rgb();
    hsv_target = properties.target;
    rgb_target = hsv_target.to_rgb();
    gradient_amount = 255.0 / static_cast<float>(properties.length);
    gradient_amount /= 255.0;
  }

  void Chroma::log_buffer(char *buffer, size_t buffer_size) const
  {
    snprintf(buffer, buffer_size,
             "Chroma:\n\tdelta=%d\n"
             "\tsource=r%u g%u b%u\th%u s%u v%u\n"
             "\ttarget=r%u g%u b%u\th%u s%u v%u\n",
             delta,
             rgb_source.red, rgb_source.green, rgb_source.blue,
             hsv_source.hue, hsv_source.saturation, hsv_source.value,
             rgb_target.red, rgb_target.green, rgb_target.blue,
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