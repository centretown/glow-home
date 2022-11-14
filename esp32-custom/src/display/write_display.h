
#pragma once

#include "esphome.h"

using namespace esphome;

void drawLines(display::DisplayBuffer &it,
               display::Font *font, float brightness,
               bool grid_on, bool strip_on);
