#pragma once

#include "esphome.h"
using namespace esphome;
using namespace light;

#include "ScannerPresets.h"
#include "Lamp.h"

namespace glow
{
  class Scanner : public Lamp
  {
  private:
    uint16_t width = 2;
    Color scan_color;
    ESPHSVColor scan_hsv_color;
    uint16_t position = 0;

  public:
    void setup(AddressableLight *it, Color current_color, Origin origin = TopLeft) override
    {
      Lamp::setup(it, current_color, origin);
      if (is_setup != 1)
        return;

      ScannerPresets presets;
      presets.setup();
      width = presets.width;
      scan_hsv_color = presets.scan_hsv_color;
      scan_color = scan_hsv_color.to_rgb();
      log();
    }

    void apply()
    {
      if (!is_ready())
        return;

      for (uint16_t i = position;
           i < position + width; i++)
      {
        uint16_t offset = map_diagonal(i % length);
        light->get(offset) = scan_color;
      }

      if (++position >= length)
      {
        position = 0;
      }

      update_hue();
      scan_color = update_hue(scan_hsv_color, delta);
    }

    void log() const override
    {
      ESP_LOGD("glow-Scanner",
               "scan_color=red:%u green:%u blue:%u",
               scan_color.red, scan_color.green, scan_color.blue);

      ESP_LOGD("glow-Scanner",
               "scan_hsv_color=hue:%u saturation:%u value:%u",
               scan_hsv_color.hue, scan_hsv_color.saturation,
               scan_hsv_color.value);
    }
  };
}
