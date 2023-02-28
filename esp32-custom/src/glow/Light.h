#pragma once
#include "base.h"
#ifdef ESPHOME_CONTROLLER
#include "esphome.h"

extern esphome::template_::TemplateNumber *update_interval;
extern esphome::template_::TemplateNumber *grid_rows;
extern esphome::template_::TemplateNumber *hue_delta;

extern esphome::template_::TemplateNumber *gradient_hue;
extern esphome::template_::TemplateNumber *gradient_saturation;
extern esphome::template_::TemplateNumber *gradient_value;

extern esphome::template_::TemplateNumber *scan_hue;
extern esphome::template_::TemplateNumber *scan_saturation;
extern esphome::template_::TemplateNumber *scan_value;
extern esphome::template_::TemplateNumber *scan_width;

namespace glow
{
  struct Light
  {
    esphome::light::AddressableLight *light = nullptr;

    bool setup(AddressableLight *p_light)
    {
      light = p_light;
      return (light != nullptr);
    }

    esphome::light::ESPColorView get(uint16_t index) const ALWAYS_INLINE
    {
      return light->get(index);
    }
  };
}
#endif
