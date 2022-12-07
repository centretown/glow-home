#pragma once

#include "esphome.h"
using namespace esphome;
using namespace light;

extern template_::TemplateNumber *gradient_hue;
extern template_::TemplateNumber *gradient_saturation;
extern template_::TemplateNumber *gradient_value;

#include "color_to_hsv.h"

namespace glow
{
  struct GradientPresets
  {
    ESPHSVColor hsv_target;

    void setup(
        float hue,
        float saturation,
        float value)
    {
      hsv_target = ESPHSVColor(static_cast<uint8_t>(hue),
                           static_cast<uint8_t>(saturation),
                           static_cast<uint8_t>(value));
    }

    void setup()
    {
      if (gradient_hue == nullptr)
      {
        ESP_LOGD("glow-GradientPresets",
                 "gradient_hue has not been initialized.");
        return;
      }

      hsv_target = ESPHSVColor(static_cast<uint8_t>(gradient_hue->state),
                           static_cast<uint8_t>(gradient_saturation->state),
                           static_cast<uint8_t>(gradient_value->state));
    }

    void log() const
    {
      ESP_LOGD("glow-GradientPresets",
               "hsv_target=hue:%u saturation:%u value:%u",
               hsv_target.hue, hsv_target.saturation, hsv_target.value);
    }
  };

}
