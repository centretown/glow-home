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

  properties.interval = 84;
  REQUIRE(properties.get("interval", buffer, sizeof(buffer)));
  printf("interval=%s\n", buffer);
  uint32_t interval = atoi(buffer);
  REQUIRE(properties.interval == interval);

  properties.scan_width = 5.0;
  REQUIRE(properties.get("scan_width", buffer, sizeof(buffer)));
  printf("scan_width=%s\n", buffer);
  uint16_t scan_width = atoi(buffer);
  REQUIRE(properties.scan_width == scan_width);

  properties.origin = Properties::BottomRight;
  REQUIRE(properties.get("origin", buffer, sizeof(buffer)));
  uint8_t origin = atoi(buffer);
  printf("origin=%s\n", buffer);
  REQUIRE(origin == Properties::BottomRight);

  properties.orientation = Properties::Diagonal;
  REQUIRE(properties.get("orientation", buffer, sizeof(buffer)));
  uint8_t orientation = atoi(buffer);
  printf("orientation=%s\n", buffer);
  REQUIRE(orientation == Properties::Diagonal);

  properties.length = 40;
  REQUIRE(properties.get("length", buffer, sizeof(buffer)));
  printf("length=%s\n", buffer);
  uint16_t length = atoi(buffer);
  REQUIRE(properties.length == length);

  properties.rows = 8;
  REQUIRE(properties.get("rows", buffer, sizeof(buffer)));
  printf("rows=%s\n", buffer);
  uint16_t rows = atoi(buffer);
  REQUIRE(properties.rows == rows);

  properties.source = ESPHSVColor(0, 255, 127);
  REQUIRE(properties.get("source", buffer, sizeof(buffer)));
  printf("source=%s\n", buffer);
  uint32_t source = atoi(buffer);
  REQUIRE(properties.hsv_to_u32(properties.source) == source);

  properties.target = ESPHSVColor(85, 0, 255);
  REQUIRE(properties.get("target", buffer, sizeof(buffer)));
  printf("target=%s\n", buffer);
  uint32_t target = atoi(buffer);
  REQUIRE(properties.hsv_to_u32(properties.target) == target);

  properties.shift = -2;
  REQUIRE(properties.get("shift", buffer, sizeof(buffer)));
  printf("shift=%s\n", buffer);
  int16_t shift = atoi(buffer);
  REQUIRE(properties.shift == shift);

  properties.transform = Properties::Monochromatic;
  REQUIRE(properties.get("transform", buffer, sizeof(buffer)));
  uint8_t transform = atoi(buffer);
  printf("transform=%s\n", buffer);
  REQUIRE(transform == Properties::Monochromatic);

  REQUIRE(false == properties.get("", buffer, sizeof(buffer)));
  REQUIRE(false == properties.get("\n", buffer, sizeof(buffer)));
  REQUIRE(false == properties.get("2", buffer, sizeof(buffer)));
  REQUIRE(false == properties.get("can_width", buffer, sizeof(buffer)));

  puts("");
}

TEST_CASE("Properties GetKey", "[properties_get_key]")
{
  Properties properties;
  char buffer[80];

  for (uint8_t key = 0; key < properties.count(); key++)
  {
    properties.get_key(key, buffer, sizeof(buffer));
    printf("%s=", buffer);
    properties.get(key, buffer, sizeof(buffer));
    printf("%s\n", buffer);
  }
}

#include "../../src/glow/Filer.h"

TEST_CASE("Filer Write/Read Properties", "[filer_write_read_properties]")
{
  Properties properties;
  properties.interval = 22;
  properties.scan_width = 5;
  properties.origin = Properties::BottomRight;
  properties.orientation = Properties::Diagonal;
  properties.length = 40;
  properties.rows = 4;
  properties.source = ESPHSVColor(127, 63, 255);
  properties.target = ESPHSVColor(255, 63, 255);
  properties.shift = -2;

  Filer filer;
  REQUIRE(filer.write("test.cfg", properties));

  Properties alternate;
  REQUIRE(filer.read("test.cfg", alternate));

  REQUIRE(properties.interval == alternate.interval);
  REQUIRE(properties.scan_width == alternate.scan_width);
  REQUIRE(properties.origin == alternate.origin);
  REQUIRE(properties.orientation == alternate.orientation);
  REQUIRE(properties.length == alternate.length);
  REQUIRE(Properties::hsv_to_u32(properties.source) ==
          Properties::hsv_to_u32(alternate.source));
  REQUIRE(Properties::hsv_to_u32(properties.target) ==
          Properties::hsv_to_u32(alternate.target));
  REQUIRE(properties.rows == alternate.rows);
  REQUIRE(properties.shift == alternate.shift);
  REQUIRE(properties.transform == alternate.transform);
}
