#include "esphome.h"
using namespace esphome;
using namespace light;

extern template_::TemplateNumber *update_interval;
extern template_::TemplateNumber *grid_rows;
extern template_::TemplateNumber *hue_delta;

extern template_::TemplateNumber *gradient_hue;
extern template_::TemplateNumber *gradient_saturation;
extern template_::TemplateNumber *gradient_value;

extern template_::TemplateNumber *scan_hue;
extern template_::TemplateNumber *scan_saturation;
extern template_::TemplateNumber *scan_value;
extern template_::TemplateNumber *scan_width;

namespace glow
{

  class Light
  {
  protected:
    AddressableLight *light = nullptr;
  };
}
