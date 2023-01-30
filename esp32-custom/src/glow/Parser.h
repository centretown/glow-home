#pragma once

#include <unordered_map>
#include <string>

#include "base.h"

namespace glow
{
  struct Parser
  {
    static std::unordered_map<std::string, uint8_t> keywords;

    enum : uint16_t
    {
      Ignore,
      Incomplete,
      Value = 4096,
      Scene,
      Layer,
    };

    uint16_t parse(char *buffer,
                   char *key, size_t key_size,
                   char *value, size_t value_size);
  };
}
