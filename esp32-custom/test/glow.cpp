#include <iostream>
#include "../src/glow/Grid.h"
#include "GlowConfig.h"

using namespace glow;

int main(int argc, char **argv)
{
  std::cout << argv[0] << " Version " << Glow_VERSION_MAJOR << "."
            << Glow_VERSION_MINOR << std::endl;
  std::cout << "Usage: " << argv[0] << " number" << std::endl;

  Grid grid;
  grid.setup(20, 4);

  const uint16_t length = grid.Length();
  for (uint16_t index = 0; index < length; index++)
  {
    uint16_t result = grid.map_diagonal(index);
    printf("index=%u result=%u\n", index, result);
  }
  return 0;
}
