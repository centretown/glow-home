#pragma once

#include "base.h"
#include "Properties.h"
#include "Grid.h"
#include "Chroma.h"
#include "Scanner.h"

namespace glow
{
  template <typename LIGHT, typename TIMER>
  class Spinner
  {
  private:
    LIGHT *light = nullptr;

  public:
    Grid grid;
    Chroma chroma;
    Scanner scanner;

    TIMER timer;
    uint32_t next = 0;
    uint32_t interval = 48;

    void setup(LIGHT *it, Properties &properties)
    {
      light = it;
      if (light == nullptr)
      {
        return;
      }

      interval = properties.interval;
      grid.setup(properties);
      chroma.setup(properties);
      scanner.setup(properties.scan_width);
    }

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

    void spin() ALWAYS_INLINE
    {
      spin(0, grid.length);
    }

    void spin(uint16_t begin, uint16_t end) ALWAYS_INLINE
    {
      for (uint16_t i = begin; i < end; ++i)
      {
        uint16_t offset = grid.map(i);
        light->get(offset) = chroma.map(i);
      }
      chroma.update_hue();
    }

    template <typename PRESENTER>
    void spin(PRESENTER &p)
    {
      spin(0, grid.length, p);
    }

    template <typename PRESENTER>
    void spin(uint16_t begin, uint16_t end, PRESENTER &p)
    {
      for (uint16_t i = begin; i < end; ++i)
      {
        Coordinates coord = grid.map_coordinates(grid.map(i));
        p.present(coord.row, coord.column, chroma.map(i));
      }
      chroma.update_hue();
    }

    void log_buffer(char *buffer, size_t buffer_size) const
    {
      snprintf(buffer, buffer_size, "Spinner:\n\tinterval=%u\n", interval);
      auto l = strlen(buffer);
      grid.log_buffer(buffer + l, buffer_size - l);
      l = strlen(buffer);
      chroma.log_buffer(buffer + l, buffer_size - l);
    }
  };
}