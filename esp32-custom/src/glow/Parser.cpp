#include "Parser.h"

namespace glow
{
  std::unordered_map<std::string, uint8_t> Parser::keywords = {
      {"layer", Layer},
  };

  uint16_t Parser::parse(char *buffer,
                        char *key, size_t key_size,
                        char *value, size_t value_size)
  {
    const char *delim = " \n\t=:,`'\"{}[]()";
    char *pch = strtok(buffer, delim);
    if (pch == NULL)
    {
      return Ignore;
    }

    if (*pch == '#')
    {
      return Ignore;
    }

    strncpy(key, pch, key_size);

    pch = strtok(NULL, delim);
    if (pch == NULL)
    {
      return Incomplete;
    }
    strncpy(value, pch, value_size);

    auto iter = keywords.find(key);
    if (iter != keywords.end())
    {
      return iter->second;
    }

    return Value;
  }
}