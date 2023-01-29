#include <iostream>
#include <signal.h>
#include <chrono>
#include <thread>

#include "GlowConfig.h"

#include "../src/glow/Properties.h"
#include "../src/glow/Filer.h"
#include "../src/glow/TestLight.h"
#include "../src/glow/TestSpinner.h"
#include "../src/glow/DisplayANSI.h"

using namespace glow;

char print_buffer[1024];

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
    // if (spinner.is_ready())
    // {
      spinner.spin(presenter);
    // }
    std::this_thread::sleep_for(std::chrono::milliseconds(spinner.interval));
  }
}

const char *file_name_default = "glow.cfg";
char file_name[132];
char arg_buffer[80];

void process_args(int argc, char **argv)
{
  int length = 0;
  for (auto i = 0; i < argc; i++)
  {
    snprintf(arg_buffer + length,
             sizeof(arg_buffer) - length,
             "%s ", argv[i]);
    length = strlen(arg_buffer);
  }

  if (argc > 1)
  {
    strncpy(file_name, argv[1], sizeof(file_name));
  }
  else
  {
    strncpy(file_name, file_name_default, sizeof(file_name));
  }
}

int main(int argc, char **argv)
{
  auto sigintHandler = [](int sig_num)
  {
    DisplayANSI::show_cursor();
    printf("\n%s\nVersion %d.%d\n%s\n",
           arg_buffer,
           Glow_VERSION_MAJOR, Glow_VERSION_MINOR,
           print_buffer);
    exit(0);
  };

  signal(SIGINT, sigintHandler);

  process_args(argc, argv);

  Properties properties;
  Filer filer;
  filer.read_create(file_name, properties);

  spin(properties);

  return 0;
}
