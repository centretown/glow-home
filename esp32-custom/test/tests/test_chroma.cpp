#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "../../src/glow/Chroma.h"

using namespace glow;

TEST_CASE("Chroma Basic", "[chroma_basic]")
{
  Chroma chroma;
  Color source{0, 255, 0};
  ESPHSVColor target(0, 255, 255);
  chroma.setup(source, target, 1);
  char buffer[80];
  chroma.log_buffer(buffer, sizeof(buffer));
  printf("%s\n", buffer);

  REQUIRE(0 == chroma.hsv_target.hue);
  REQUIRE(255 == chroma.hsv_target.saturation);
  REQUIRE(255 == chroma.hsv_target.value);
}
