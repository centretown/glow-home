#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "../../src/glow/Chroma.h"

using namespace glow;

TEST_CASE("Grid Map Diagonal 2x2", "[map_2x2]")
{
  Chroma chroma;
  chroma.setup(0, 1, 2);

  REQUIRE(0 == chroma.hsv_target.hue);
  REQUIRE(1 == chroma.hsv_target.saturation);
  REQUIRE(2 == chroma.hsv_target.value);
}
