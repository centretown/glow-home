#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "../../src/glow/Properties.h"
#include "../../src/glow/TestLight.h"
#include "../../src/glow/TestSpinner.h"
#include "../../src/glow/DisplayANSI.h"

using namespace glow;

char print_buffer[1024];

void log_colors(TestLight &light, Properties &properties)
{
  const uint16_t columns = properties.length / properties.grid_rows;

  for (auto i = 0; i < properties.length; i++)
  {
    if (i % columns == 0)
    {
      puts("");
    }
    auto c = light.get(i);
    printf("\x1b[38;2;%d;%d;%dm%02x%02x%02x \x1b[0m",
           c.r, c.g, c.b, c.r, c.g, c.b);
  }
  puts("");
}

TEST_CASE("Spinner Basic", "[spinner_basic]")
{
  TestLight light;
  TestSpinner spinner;

  Properties properties;
  properties.length = 36;
  properties.grid_rows = 4;

  Color source(255, 0, 0);
  ESPHSVColor target(170, 0, 255);
  properties.current_color = source;
  properties.gradient_hue = target.hue;
  properties.gradient_saturation = target.saturation;
  properties.gradient_value = target.value;

  spinner.setup(&light, properties);

  spinner.log_buffer(print_buffer, sizeof(print_buffer));
  printf("%s\n", print_buffer);

  spinner.spin();
  log_colors(light, properties);

  properties.orientation = Vertical;
  spinner.setup(&light, properties);
  spinner.spin();
  log_colors(light, properties);

  properties.orientation = Diagonal;
  spinner.setup(&light, properties);
  spinner.spin();
  log_colors(light, properties);

  // DisplayANSI presenter;
  // presenter.setup();
  // for (auto counter = 0; counter < 256;)
  // // while (true)
  // {
  //   if (spinner.is_ready())
  //   {
  //     spinner.spin(presenter);
  //     ++counter;
  //   }
  // }
  // presenter.show_cursor();
  // puts("");
  // spinner.log_buffer(print_buffer, sizeof(print_buffer));
  // puts(print_buffer);
}
