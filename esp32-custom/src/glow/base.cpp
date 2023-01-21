#ifndef USE_ESP32

#include "base.h"

float gamma_correct(float value, float gamma)
{
  if (value <= 0.0f)
    return 0.0f;
  if (gamma <= 0.0f)
    return value;

  return powf(value, gamma);
}

float gamma_uncorrect(float value, float gamma)
{
  if (value <= 0.0f)
    return 0.0f;
  if (gamma <= 0.0f)
    return value;

  return powf(value, 1 / gamma);
}

void rgb_to_hsv(float red, float green, float blue, int &hue, float &saturation, float &value)
{
  float max_color_value = std::max(std::max(red, green), blue);
  float min_color_value = std::min(std::min(red, green), blue);
  float delta = max_color_value - min_color_value;

  if (delta == 0)
  {
    hue = 0;
  }
  else if (max_color_value == red)
  {
    hue = int(fmod(((60 * ((green - blue) / delta)) + 360), 360));
  }
  else if (max_color_value == green)
  {
    hue = int(fmod(((60 * ((blue - red) / delta)) + 120), 360));
  }
  else if (max_color_value == blue)
  {
    hue = int(fmod(((60 * ((red - green) / delta)) + 240), 360));
  }

  if (max_color_value == 0)
  {
    saturation = 0;
  }
  else
  {
    saturation = delta / max_color_value;
  }

  value = max_color_value;
}
void hsv_to_rgb(int hue, float saturation, float value, float &red, float &green, float &blue)
{
  float chroma = value * saturation;
  float hue_prime = fmod(hue / 60.0, 6);
  float intermediate = chroma * (1 - fabs(fmod(hue_prime, 2) - 1));
  float delta = value - chroma;

  if (0 <= hue_prime && hue_prime < 1)
  {
    red = chroma;
    green = intermediate;
    blue = 0;
  }
  else if (1 <= hue_prime && hue_prime < 2)
  {
    red = intermediate;
    green = chroma;
    blue = 0;
  }
  else if (2 <= hue_prime && hue_prime < 3)
  {
    red = 0;
    green = chroma;
    blue = intermediate;
  }
  else if (3 <= hue_prime && hue_prime < 4)
  {
    red = 0;
    green = intermediate;
    blue = chroma;
  }
  else if (4 <= hue_prime && hue_prime < 5)
  {
    red = intermediate;
    green = 0;
    blue = chroma;
  }
  else if (5 <= hue_prime && hue_prime < 6)
  {
    red = chroma;
    green = 0;
    blue = intermediate;
  }
  else
  {
    red = 0;
    green = 0;
    blue = 0;
  }

  red += delta;
  green += delta;
  blue += delta;
}

uint16_t millis_rate = 10;
uint64_t millis_now = 0;
bool real_time = false;

void set_millis_rate(uint16_t r)
{
  millis_rate = r;
}

uint32_t micros32()
{
  return static_cast<uint32_t>(clock());
}

uint32_t millis32()
{
  clock_t lms = clock();
  lms /= clocks_ms;
  return static_cast<uint32_t>(lms);
}

#endif
