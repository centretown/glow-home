#include "write_lights.h"

ESPHSVColor current_hsv_color(Color &current_color)
{
  float red = (float)current_color.red / 255.0;
  float green = (float)current_color.green / 255.0;
  float blue = (float)current_color.blue / 255.0;
  float saturation, value;
  int hue;
  rgb_to_hsv(red, green, blue, hue, saturation, value);
  hue *= 255;
  hue /= 360;
  saturation *= 255;
  value *= 255;

  return ESPHSVColor((uint8_t)hue, (uint8_t)saturation, (uint8_t)value);
}

void spin(AddressableLight &light,
          ESPHSVColor hsv, uint8_t &hsv_element,
          const uint16_t vary)
{
  uint16_t val = 0xFFFF;

  for (auto pixel : light)
  {
    hsv_element = val >> 8;
    pixel = hsv;
    val -= vary;
  }
}

void hsvSaturation(AddressableLight &light, int &hue, bool initial_run)
{

  ESPHSVColor hsv;
  hsv.hue = hue;
  hsv.value = 255;
  uint16_t sat = 0xFFFF;
  const uint16_t add = 0xBFFF / light.size();

  spin(light, hsv, hsv.saturation, add);

  // for (auto pixel : light)
  // {
  //   hsv.value = sat >> 8;
  //   pixel = hsv;
  //   sat -= add;
  // }

  hue += 1;
}

void hsvValue(AddressableLight &light, int &hue, bool initial_run)
{
  ESPHSVColor hsv;

  hsv.hue = hue;
  hsv.saturation = 240;
  uint16_t val = 0xFFFF;
  const uint16_t add = 0x7FFF / light.size();

  spin(light, hsv, hsv.saturation, add);

  // for (auto pixel : light)
  // {
  //   hsv.value = val >> 8;
  //   pixel = hsv;
  //   val -= add;
  // }

  hue += 1;
}
