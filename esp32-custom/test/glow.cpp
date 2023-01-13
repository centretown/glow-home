#include <iostream>
#include "GlowConfig.h"

int main(int argc, char **argv)
{
  std::cout << argv[0] << " Version " << Glow_VERSION_MAJOR << "."
            << Glow_VERSION_MINOR << std::endl;
  std::cout << "Usage: " << argv[0] << " number" << std::endl;
  return 0;
}
