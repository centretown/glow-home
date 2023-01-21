#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "../../src/glow/Properties.h"
#include "../../src/glow/TestLight.h"
#include "../../src/glow/TestSpinner.h"

using namespace glow;

TEST_CASE("Spinner Basic", "[spinner_basic]")
{
  TestLight light;
  TestSpinner spinner;

  Properties properties;
  properties.length = 36;
  spinner.setup(&light, properties);

  char buffer[80];
  spinner.grid.log_buffer(buffer, sizeof(buffer));
  printf("%s\n", buffer);
  spinner.chroma.log_buffer(buffer, sizeof(buffer));
  printf("%s\n", buffer);


}
