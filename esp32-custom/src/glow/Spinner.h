#pragma once

#include "Properties.h"
#include "Grid.h"
#include "Chroma.h"
#include "Scanner.h"
#include "color_to_hsv.h"

#ifndef ALWAYS_INLINE
#define ALWAYS_INLINE __attribute__((always_inline))
#endif

namespace glow
{
  template <typename TIMER, typename LIGHT>
  class Spinner
  {
  private:
    LIGHT *light = nullptr;

    Grid grid;
    Chroma chroma;
    Scanner scanner;

    TIMER timer;
    uint32_t next = 0;
    uint32_t interval = 48;

  public:
    bool is_ready() ALWAYS_INLINE
    {
      uint32_t now = timer.now();
      if (next - now > interval)
      {
        next = now + interval;
        return true;
      }
      return false;
    }

    template <typename COLOR_MAPPER>
    void spin(uint16_t begin, uint16_t end,
              COLOR_MAPPER &color_mapper)
    {
      for (uint16_t i = begin; i < end; ++i)
      {
        light->get(i) = color_mapper(i);
      }
      chroma.update_hue();
    }

    template <typename MAPPER, typename COLOR_MAPPER>
    void spin(uint16_t begin, uint16_t end,
              MAPPER &mapper, COLOR_MAPPER &color_mapper)
    {
      for (uint16_t i = begin; i < end; ++i)
      {
        uint16_t offset = mapper(i);
        light->get(offset) = color_mapper(i);
      }
      chroma.update_hue();
    }
  };
}