#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "../../src/glow/Properties.h"

using namespace glow;

char print_buffer[1024];

TEST_CASE("Properties Basic", "[properties_basic]")
{
  Color current_color(15, 236, 7);

  Properties properties;

  char buffer[80];

  properties.current_color = current_color;
  properties.get("current_color", buffer, sizeof(buffer));
  uint32_t colorraw = atoi(buffer);
  printf("current_color=%s\n", buffer);
  REQUIRE(current_color.raw_32 == colorraw);

  properties.origin = BottomRight;
  properties.get("origin", buffer, sizeof(buffer));
  uint32_t origin = atoi(buffer);
  printf("origin=%s\n", buffer);
  REQUIRE(origin == BottomRight);

  properties.orientation = Diagonal;
  properties.get("orientation", buffer, sizeof(buffer));
  uint32_t orientation = atoi(buffer);
  printf("orientation=%s\n", buffer);
  REQUIRE(orientation == Diagonal);

  properties.length = 40;
  properties.get("length", buffer, sizeof(buffer));
  printf("length=%s\n", buffer);
  uint16_t length = atoi(buffer);
  REQUIRE(properties.length == length);

  properties.grid_rows = 4;
  properties.get("grid_rows", buffer, sizeof(buffer));
  printf("grid_rows=%s\n", buffer);
  float grid_rows = atof(buffer);
  REQUIRE(properties.grid_rows == grid_rows);

  properties.hue_delta = -2.0;
  properties.get("hue_delta", buffer, sizeof(buffer));
  printf("hue_delta=%s\n", buffer);
  float hue_delta = atof(buffer);
  REQUIRE(properties.hue_delta == hue_delta);

  properties.gradient_hue = 180.0;
  properties.get("gradient_hue", buffer, sizeof(buffer));
  printf("gradient_hue=%s\n", buffer);
  float gradient_hue = atof(buffer);
  REQUIRE(properties.gradient_hue == gradient_hue);

  properties.gradient_saturation = 127.0;
  properties.get("gradient_saturation", buffer, sizeof(buffer));
  printf("gradient_saturation=%s\n", buffer);
  float gradient_saturation = atof(buffer);
  REQUIRE(properties.gradient_saturation == gradient_saturation);

  properties.gradient_value = 67.0;
  properties.get("gradient_value", buffer, sizeof(buffer));
  printf("gradient_value=%s\n", buffer);
  float gradient_value = atof(buffer);
  REQUIRE(properties.gradient_value == gradient_value);

  properties.scan_width = 5.0;
  properties.get("scan_width", buffer, sizeof(buffer));
  printf("scan_width=%s\n", buffer);
  float scan_width = atof(buffer);
  REQUIRE(properties.scan_width == scan_width);

  puts("");
}

TEST_CASE("Properties GetKey", "[properties_get_key]")
{
  Properties properties;
  char buffer[80];

  for (uint8_t key = 0; key < PROPERTY_COUNT; key++)
  {
    properties.get_key(key, buffer, sizeof(buffer));
    printf("%s=", buffer);
    properties.get(key, buffer, sizeof(buffer));
    printf("%s\n", buffer);
  }
}

#include "../../src/glow/Filer.h"

TEST_CASE("Properties Write", "[properties_write]")
{
  Color current_color(15, 236, 7);
  Properties properties;
  properties.current_color = current_color;
  properties.origin = BottomRight;
  properties.orientation = Diagonal;
  properties.length = 40;
  properties.grid_rows = 4;
  properties.hue_delta = -2.0;
  properties.gradient_hue = 180.0;
  properties.gradient_saturation = 127.0;
  properties.gradient_value = 67.0;
  properties.scan_width = 5.0;

  Filer filer;
  filer.write("test.cfg", properties);

  Properties target;
  filer.read("test.cfg", target);

  REQUIRE(properties.current_color.raw_32 == target.current_color.raw_32);
  REQUIRE(properties.origin == target.origin);
  REQUIRE(properties.orientation == target.orientation);
  REQUIRE(properties.length == target.length);
  REQUIRE(properties.grid_rows == target.grid_rows);
  REQUIRE(properties.hue_delta == target.hue_delta);
  REQUIRE(properties.gradient_hue == target.gradient_hue);
  REQUIRE(properties.gradient_saturation == target.gradient_saturation);
  REQUIRE(properties.gradient_value == target.gradient_value);
  REQUIRE(properties.scan_width == target.scan_width);
}
