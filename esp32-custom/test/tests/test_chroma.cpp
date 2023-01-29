#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include <catch2/catch.hpp>

#include "../../src/glow/HSVColor.h"
#include "../../src/glow/Properties.h"
#include "../../src/glow/Chroma.h"

using namespace glow;

static char print_buffer[512];

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
  properties.set_grid(36, 4, Properties::TopLeft, Properties::Diagonal);
  Chroma chroma;

  ESPHSVColor source(0, 255, 255);
  ESPHSVColor target(128, 0, 127);
  properties.source = source;
  properties.target = target;

  chroma.setup(properties);
  chroma.log_buffer(print_buffer, sizeof(print_buffer));
  puts(print_buffer);

  REQUIRE(hsv_to_u32(properties.target) ==
          hsv_to_u32(chroma.hsv_target));

  for (int i = 0; i < sizeof(rgbColors) / sizeof(rgbColors[0]); i++)
  {
    Color &color = rgbColors[i];
    ESPHSVColor quick = color_to_hsv(color);
    ESPHSVColor slow = old_color_to_hsv(color);
    chroma.log_rgb(print_buffer, sizeof(print_buffer), color);
    printf("%s", print_buffer);
    chroma.log_hsv(print_buffer, sizeof(print_buffer), slow);
    printf("%s", print_buffer);
    chroma.log_hsv(print_buffer, sizeof(print_buffer), quick);
    printf("%s", print_buffer);
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
  //   return old_color_to_hsv(rgbBench);
  // };

  // BENCHMARK("quick")
  // {
  //   return color_to_hsv(rgbBench);
  // };
}
