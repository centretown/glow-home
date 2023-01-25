#include <iostream>
#include <signal.h>

#include "GlowConfig.h"

#include "../src/glow/Properties.h"
#include "../src/glow/Filer.h"
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
  properties.hue_delta = -1;

  Color source(255, 0, 255);
  ESPHSVColor target(170, 0, 255);
  properties.current_color = source;
  properties.gradient_hue = target.hue;
  properties.gradient_saturation = target.saturation;
  properties.gradient_value = target.value;

  spinner.setup(&light, properties);
  spinner.log_buffer(print_buffer, sizeof(print_buffer));

  DisplayANSI presenter;
  // const char *bullet = "\U000025CD";
  // presenter.setup(bullet);
  presenter.setup();

  while (true)
  {
    if (spinner.is_ready())
    {
      spinner.spin(presenter);
    }
  }
}

void read(Properties &properties)
{
}

void spin(Properties &properties)
{
  TestLight light;
  TestSpinner spinner;
  spinner.setup(&light, properties);
  spinner.log_buffer(print_buffer, sizeof(print_buffer));

  DisplayANSI presenter;
  presenter.setup();

  while (true)
  {
    if (spinner.is_ready())
    {
      spinner.spin(presenter);
    }
  }
}

int main(int argc, char **argv)
{
  auto sigintHandler = [](int sig_num)
  {
    DisplayANSI::show_cursor();
    puts("");
    puts(print_buffer);
    printf("Version %d.%d\n",
           Glow_VERSION_MAJOR, Glow_VERSION_MINOR);
    exit(0);
  };

  signal(SIGINT, sigintHandler);

  Properties properties;
  Filer filer;
  filer.read("glow.cfg", properties);
  spin(properties);

  // showStuff();
  return 0;
}
