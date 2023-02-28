#include "Layer.h"

namespace glow
{
#ifndef MICRO_CONTROLLER
  std::string Layer::make_code()
  {
    std::stringstream s;
    s << "{" << length << ","
      << rows << ","
      << grid.make_code() << ","
      << chroma.make_code() << ","
      << hue_shift << ","
      << scan << ","
      << begin << ","
      << end << "}";
    return s.str();
  }

  std::string Layer::keys[Layer::KEY_COUNT] = {
      "length",
      "rows",
      "grid",
      "chroma",
      "hue_shift",
      "scan",
      "begin",
      "end",
  };
#endif
  uint16_t Layer::calculate_bounds(uint16_t bound)
  {
    if (bound > 100)
    {
      bound %= 100;
    }

    float amt = static_cast<float>(bound) / 100.0f *
                static_cast<float>(length);
    return grid.adjust_bounds(amt);
  }

}