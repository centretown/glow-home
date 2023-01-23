#pragma once

#include "base.h"
#include "../esphome/core/color.h"
using esphome::Color;

namespace glow
{
  const char *circle = "\U000025CD";
  class DisplayANSI
  {
  private:
    /* data */
  public:
    void present(uint16_t row, uint16_t column, Color color)
    {
      cursor(row + 1, (column + 1) * 2);
      printf("\x1b[38;2;%d;%d;%dm%s \x1b[0m",
             color.r, color.g, color.b, circle);
    }

    static void cursor(uint16_t row, uint16_t column)
    {
      printf("\x1b[%d;%dH\x1b[0m", row, column);
    }

    static void hide_cursor()
    {
      printf("\x1b[?25l\x1b[0m");
    }

    static void show_cursor()
    {
      printf("\x1b[?25h\x1b[0m");
    }

    void setup()
    {
      printf("\x1b[%d;%dH\x1b[0m", 0, 0);
      printf("\x1b[0J\x1b[0m");
      hide_cursor();
    }
  };
}
