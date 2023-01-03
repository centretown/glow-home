#pragma once

#include "esphome.h"
using namespace esphome;
using namespace light;

#include "Lamp.h"
#include "Presets.h"

namespace glow
{
  class Solid : public Lamp
  {
  public:
    void setup(AddressableLight *it, Color current_color, uint16_t origin = 0)
    {
      Lamp::setup(it, current_color, origin);
      log();
    }

    void apply()
    {
      if (is_ready())
      {
        light->all().set(color);
        update_hue();
      }
    }
  };
}
