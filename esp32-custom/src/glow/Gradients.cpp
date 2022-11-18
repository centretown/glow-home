
#include "Gradients.h"

namespace glow
{
  ESPHSVColor Gradients::hsv_color(Color color)
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

  Color Gradients::color_gradient(const Color &from, const Color &to, uint8_t amnt)
  {
    Color color;
    float amnt_f = float(amnt) / 255.0f;
    color.r = amnt_f * (to.r - from.r) + from.r;
    color.g = amnt_f * (to.g - from.g) + from.g;
    color.b = amnt_f * (to.b - from.b) + from.b;
    return color;
  }
}