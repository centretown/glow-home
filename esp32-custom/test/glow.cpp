#include <iostream>
#include "GlowConfig.h"

#include "../src/glow/base.h"
#include "../src/glow/Properties.h"
#include "../src/glow/TestLight.h"
#include "../src/glow/TestSpinner.h"
#include "../src/glow/DisplayANSI.h"

using namespace glow;

char print_buffer[1024];

void showStuff()
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

  DisplayANSI presenter;
  presenter.setup();
  for (auto counter = 0; counter < 256;)
  // while (true)
  {
    if (spinner.is_ready())
    {
      spinner.spin(presenter);
      ++counter;
    }
  }
  presenter.show_cursor();
  puts("");
  spinner.log_buffer(print_buffer, sizeof(print_buffer));
  puts(print_buffer);
}

int main(int argc, char **argv)
{
  showStuff();

  std::cout << argv[0] << " Version " << Glow_VERSION_MAJOR << "."
            << Glow_VERSION_MINOR << std::endl;
  std::cout << "Usage: " << argv[0] << " number" << std::endl;
  return 0;
}
