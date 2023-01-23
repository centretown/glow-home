#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include <catch2/catch.hpp>

#include "../../src/glow/Chroma.h"

using namespace glow;

Color rgbColors[] = {
    Color(254, 255, 255),
    Color(255, 255, 0),
    Color(255, 0, 255),
    Color(255, 0, 0),
    Color(0, 255, 255),
    Color(0, 255, 0),
    Color(0, 0, 255),
    Color(0, 0, 0),
    Color(127, 127, 127),
    Color(127, 127, 0),
    Color(126, 0, 127),
    Color(127, 0, 0),
    Color(0, 127, 127),
    Color(0, 127, 0),
    Color(0, 0, 127),
    Color(127, 63, 63),
    Color(127, 63, 0),
    Color(127, 0, 63),
    Color(127, 0, 0),
    Color(0, 127, 63),
    Color(0, 127, 0),
    Color(0, 0, 127),
    Color(11, 10, 11),
    Color(11, 11, 10),
};

TEST_CASE("Chroma Basic", "[chroma_basic]")
{
  Properties properties;
  properties(36.0, 4.0, TopLeft, Diagonal);
  Chroma chroma;

  properties.current_color = Color{0, 255, 0};
  ESPHSVColor target(128, 255, 255);
  properties.gradient_hue = target.hue;
  properties.gradient_saturation = target.saturation;
  properties.gradient_value = target.value;

  chroma.setup(properties);
  static char buffer[512];
  chroma.log_buffer(buffer, sizeof(buffer));
  printf("%s\n", buffer);

  REQUIRE(properties.gradient_hue == chroma.hsv_target.hue);
  REQUIRE(properties.gradient_saturation == chroma.hsv_target.saturation);
  REQUIRE(properties.gradient_value == chroma.hsv_target.value);

  for (int i = 0; i < sizeof(rgbColors) / sizeof(rgbColors[0]); i++)
  {
    Color &color = rgbColors[i];
    ESPHSVColor quick = Chroma::color_to_hsv(color);
    ESPHSVColor slow = Chroma::old_color_to_hsv(color);
    chroma.log_rgb(buffer, sizeof(buffer), color);
    printf("%s", buffer);
    chroma.log_hsv(buffer, sizeof(buffer), slow);
    printf("%s", buffer);
    chroma.log_hsv(buffer, sizeof(buffer), quick);
    printf("%s", buffer);
    int8_t hue_diff = slow.hue - quick.hue;
    int8_t sat_diff = slow.saturation - quick.saturation;
    int8_t val_diff = slow.value - quick.value;
    // CHECK((hue_diff == 0));
    // CHECK((sat_diff == 0));
    // CHECK((val_diff == 0));
    CHECK((hue_diff <= 1 && hue_diff >= -1));
    CHECK((sat_diff <= 1 && sat_diff >= -1));
    CHECK((val_diff <= 1 && val_diff >= -1));
  }

  // Color rgbBench(0, 242, 14);
  // BENCHMARK("slow")
  // {
  //   return Chroma::old_color_to_hsv(rgbBench);
  // };

  // BENCHMARK("quick")
  // {
  //   return Chroma::color_to_hsv(rgbBench);
  // };
}
