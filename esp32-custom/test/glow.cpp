#include <iostream>
#include "include/grid.h"
#include "GlowConfig.h"

using namespace glow;

int main(int argc, char **argv)
{
  std::cout << argv[0] << " Version " << Glow_VERSION_MAJOR << "."
            << Glow_VERSION_MINOR << std::endl;
  std::cout << "Usage: " << argv[0] << " number" << std::endl;

  Grid grid(4, 5);

  for (uint16_t index = 0; index < grid.length; index++)
  {
    uint16_t result = grid.map_diagonal(index);
    printf("index=%u result=%u\n", index, result);
  }
  return 0;
}
