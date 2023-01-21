#pragma once

#include "base.h"
#include "Properties.h"

namespace glow
{
  constexpr uint16_t byte_limit = 0xff;
  constexpr uint16_t hue_limit = 1530;
  constexpr uint16_t hue_segment = hue_limit / 6;

  constexpr uint16_t hue_red = 0;
  constexpr uint16_t hue_yellow = hue_segment;
  constexpr uint16_t hue_green = hue_limit / 3;
  constexpr uint16_t hue_cyan = hue_limit / 2;
  constexpr uint16_t hue_blue = hue_limit * 2 / 3;
  constexpr uint16_t hue_magenta = hue_limit * 5 / 6;

  class Chroma
  {
    // private:
  public:
    Color rgb_source;
    ESPHSVColor hsv_source;

    Color rgb_target;
    ESPHSVColor hsv_target;

    int16_t delta = 1;

  public:
    void setup(Properties &properties);

    void update_hue() ALWAYS_INLINE
    {
      if (delta == 0)
        return;
      hsv_source.hue += delta;
      rgb_source = hsv_source.to_rgb();
    }

    Color step_gradient(uint8_t amnt) ALWAYS_INLINE
    {
      Color step;
      float amnt_f = static_cast<float>(amnt) / 255.0f;
      step.r = rgb_source.r + amnt_f * (rgb_target.r - rgb_source.r);
      step.g = rgb_source.g + amnt_f * (rgb_target.g - rgb_source.g);
      step.b = rgb_source.b + amnt_f * (rgb_target.b - rgb_source.b);
      return step;
    }

    static ESPHSVColor color_to_hsv(Color color);
#ifndef USE_ESP32
    // keep original for testing
    static ESPHSVColor old_color_to_hsv(Color color);
#endif
    void log_buffer(char *buffer, size_t buffer_size) const;
    static char *log_hsv(char *buffer, size_t buffer_size, ESPHSVColor hsv);
    static char *log_rgb(char *buffer, size_t buffer_size, Color rgb);
  };

}
