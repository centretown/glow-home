#include "RGBColor.h"

namespace glow
{
#ifndef MICRO_CONTROLLER
  std::string Color::keys[KEY_COUNT] = {
      "red",
      "green",
      "blue",
  };
#endif // MICRO_CONTROLLER
}