#pragma once

#include "base.h"
#include "../esphome/core/color.h"
using esphome::Color;

namespace glow
{
  const char *bullet = "\U000025CF";
  class DisplayANSI
  {
  private:
    char element[16];
    uint16_t width = 2;

  public:
    DisplayANSI()
    {
      strncpy(element, bullet, sizeof(element));
    }

    static void at(uint16_t row, uint16_t column) ALWAYS_INLINE
    {
      printf("\x1b[%d;%dH\x1b[0m", row, column);
    }

    static void hide_cursor() ALWAYS_INLINE
    {
      printf("\x1b[?25l\x1b[0m");
    }

    static void show_cursor() ALWAYS_INLINE
    {
      printf("\x1b[?25h\x1b[0m");
    }

    static void clear_from_cursor() ALWAYS_INLINE
    {
      printf("\x1b[0J\x1b[0m");
    }

  public:
    void present(uint16_t row, uint16_t column, Color color) ALWAYS_INLINE
    {
      at(row + 1, (column + 1) * width);
      printf("\x1b[38;2;%d;%d;%dm%s \x1b[0m",
             color.r, color.g, color.b, element);
    }

    void setup(const char *el = nullptr, uint16_t w = 2)
    {
      width = w;
      if (el != nullptr)
      {
        strncpy(element, el, sizeof(element));
      }
      at(1, 1);
      clear_from_cursor();
      hide_cursor();
    }
  };
}
