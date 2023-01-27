#pragma once

#include "base.h"
// #include <thread> // std::this_thread::sleep_for
// #include <chrono> // std::chrono::seconds
namespace glow
{
  class TestTimer
  {
  public:
    uint32_t now() ALWAYS_INLINE
    {
      // std::this_thread::sleep_for(std::chrono::milliseconds(1));
      return millis32();
    }
  };

} // namespace glow
