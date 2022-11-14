
#include "write_display.h"

void drawLines(display::DisplayBuffer &screen,
               display::Font *font, float brightness,
               bool grid_on, bool strip_on)
{
  const char *off = "OFF";
  const char *on = "ON";
  screen.printf(0, 2, font, "Grid:  %s",
                (grid_on) ? on : off);
  screen.printf(0, 18, font, "Strip:  %s",
                (strip_on) ? on : off);
  screen.printf(64, 0, font, "Brightness");
  screen.printf(74, 16, font, "%.0f %%", brightness);
  screen.line(58, 0, 58, 30);
  screen.line(0, 16, 58, 16);
}

