#pragma once

#include "esphome.h"
using namespace esphome;
using namespace light;

extern template_::TemplateNumber *scan_hue;
extern template_::TemplateNumber *scan_saturation;
extern template_::TemplateNumber *scan_value;
extern template_::TemplateNumber *scan_width;

#include "color_to_hsv.h"

namespace glow
{
  struct ScannerPresets
  {
    uint16_t width = 2;
    ESPHSVColor scan_hsv_color;

    void setup()
    {
      if (scan_hue == nullptr)
      {
        ESP_LOGD("glow-ScanPresets",
                 "gradient_hue has not been initialized.");
        return;
      }
      scan_hsv_color = ESPHSVColor(static_cast<uint8_t>(scan_hue->state),
                                   static_cast<uint8_t>(scan_saturation->state),
                                   static_cast<uint8_t>(scan_value->state));
      width = static_cast<uint16_t>(scan_width->state);
    }

    void log() const
    {
      ESP_LOGD("glow-ScannerPresets", "width=%u", width);
      ESP_LOGD("glow-ScannerPresets",
               "scan_hsv=hue:%u saturation:%u value:%u",
               scan_hsv_color.hue, scan_hsv_color.saturation,
               scan_hsv_color.value);
    }
  };
}