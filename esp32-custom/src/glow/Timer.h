#pragma once

#include "esphome.h"
using namespace esphome;

namespace glow
{
  class Timer
  {
  public:
    uint32_t now() ALWAYS_INLINE
    {
#define millis() esphome::millis()
      return millis();
    }
  };

} // namespace glow
