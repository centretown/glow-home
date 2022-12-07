#pragma once

#include <utility>

#include "esphome.h"
using namespace esphome;
using namespace light;

namespace glow
{
  template <typename Background, typename Foreground>
  class Composite
  {
  private:
    Background &background;
    Foreground &foreground;

  public:
    Composite(Background &background,
              Foreground &foreground)
        : background(background),
          foreground(foreground) {}

    void setup(AddressableLight *it, Color current_color)
    {
      // background.setup(it, current_color);
      // foreground.setup(it, current_color);
    }

    void apply()
    {
      // background.apply();
      // foreground.apply();
    }
  };

}
