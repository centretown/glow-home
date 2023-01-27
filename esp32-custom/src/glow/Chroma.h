#pragma once

#include "base.h"
#include "Properties.h"

namespace glow
{
  class Chroma
  {
    // private:
  public:
    Color rgb_source;
    Color rgb_target;
    ESPHSVColor hsv_source;
    ESPHSVColor hsv_target;

    int16_t delta = 1;
    float gradient_amount = 1.0;

  public:
    void setup(Properties &properties);

    Color map(uint16_t index)
    {
      return step_gradient(static_cast<float>(index) * gradient_amount);
    }

    void update_hue() ALWAYS_INLINE
    {
      if (delta == 0)
        return;
      hsv_source.hue += delta;
      rgb_source = hsv_source.to_rgb();
      hsv_target.hue += delta;
      rgb_target = hsv_target.to_rgb();
    }

    Color step_gradient(float shift_amount) ALWAYS_INLINE
    {
      return Color(red_shift(shift_amount),
                   green_shift(shift_amount),
                   blue_shift(shift_amount));
    }

    uint8_t red_shift(float shift_amount) ALWAYS_INLINE
    {
      shift_amount *= static_cast<float>(rgb_target.red - rgb_source.red);
      return rgb_source.red + static_cast<uint8_t>(shift_amount);
    }
    uint8_t green_shift(float shift_amount) ALWAYS_INLINE
    {
      shift_amount *= static_cast<float>(rgb_target.green - rgb_source.green);
      return rgb_source.green + static_cast<uint8_t>(shift_amount);
    }
    uint8_t blue_shift(float shift_amount) ALWAYS_INLINE
    {
      shift_amount *= static_cast<float>(rgb_target.blue - rgb_source.blue);
      return rgb_source.blue + static_cast<uint8_t>(shift_amount);
    }

    void log_buffer(char *buffer, size_t buffer_size) const;
    static char *log_hsv(char *buffer, size_t buffer_size, ESPHSVColor hsv);
    static char *log_rgb(char *buffer, size_t buffer_size, Color rgb);
  };

}
