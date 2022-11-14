#pragma once

#include "esphome.h"
using namespace esphome;
using namespace light;

namespace glow
{
  class Pixels
  {
  private:
    AddressableLight *light = nullptr;

  public:
    inline void setup(AddressableLight *it)
    {
      light = it;
    }

    inline void Put(uint16_t index, ESPHSVColor value)
    {
      light->get(index) = value;
    }

    inline void Put(uint16_t index, Color &value)
    {
      light->get(index) = value;
    }

    inline void Update() {}

    inline uint16_t size()
    {
      if (light == nullptr)
      {
        return 0;
      }

      return static_cast<uint16_t>(light->size());
    }
  };

  ESPHSVColor hsv_color(Color color);
}
