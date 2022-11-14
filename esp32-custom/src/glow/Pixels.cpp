
#include "Pixels.h"

namespace glow
{
  ESPHSVColor hsv_color(Color color)
  {
    float red = (float)color.red / 255.0;
    float green = (float)color.green / 255.0;
    float blue = (float)color.blue / 255.0;
    float saturation, value;
    int hue;
    rgb_to_hsv(red, green, blue, hue, saturation, value);
    hue *= 255;
    hue /= 360;
    saturation *= 255;
    value *= 255;

    return ESPHSVColor((uint8_t)hue, (uint8_t)saturation, (uint8_t)value);
  }

  // void testRed(AddressableLight &light, ESPHSVColor hsv)
  // {
  // }

}