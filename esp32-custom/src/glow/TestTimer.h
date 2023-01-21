#pragma once

#include "base.h"

namespace glow
{
  class TestTimer
  {
  public:
    uint32_t now() ALWAYS_INLINE
    {
      return millis32();
    }
  };

} // namespace glow
