#pragma once

#include "esphome.h"
using namespace esphome;
using namespace light;

extern template_::TemplateNumber *update_interval;
extern template_::TemplateNumber *grid_rows;
extern template_::TemplateNumber *hue_delta;

namespace glow
{
  struct Presets
  {
    uint16_t rows = 1;
    uint32_t interval = 48;
    int16_t delta = 0;

    void setup()
    {
      interval = static_cast<uint32_t>(update_interval->state);
      rows = static_cast<uint16_t>(grid_rows->state);
      delta = static_cast<int16_t>(hue_delta->state);
    }

    virtual void log() const
    {
      ESP_LOGD("glow-Presets",
               "rows=%u, interval=%u, delta=%d",
               rows, interval, delta);
    }
  };

}
