#pragma once

#include "esphome.h"
using namespace esphome;
using namespace light;

#include "Lamp.h"
#include "GradientPresets.h"

namespace glow
{
  class Gradient : public Lamp
  {
  protected:
    Color target;
    ESPHSVColor hsv_target;

  public:
    void setup(AddressableLight *it, Color current_color, uint8_t origin = 0) override
    {
      Lamp::setup(it, current_color, origin);
      if (!is_setup)
        return;

      Lamp::log();

      GradientPresets presets;
      presets.setup();

      hsv_target = presets.hsv_target;
      target = hsv_target.to_rgb();
      presets.log();
      log();
    }

    Color step_gradient(uint8_t amnt) ALWAYS_INLINE
    {
      float amnt_f = static_cast<float>(amnt) / 255.0f;
      Color step;
      step.r = color.r + amnt_f * (target.r - color.r);
      step.g = color.g + amnt_f * (target.g - color.g);
      step.b = color.b + amnt_f * (target.b - color.b);
      return step;
    }

    // log

    void log() const override
    {
      ESP_LOGD("glow-Gradient",
               "end=red:%u green:%u blue:%u",
               target.red, target.green, target.blue);

      ESP_LOGD("glow-Gradient",
               "hsv_color=hue:%u saturation:%u value:%u",
               hsv_target.hue, hsv_target.saturation,
               hsv_target.value);
    }
  };
}
