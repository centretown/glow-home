#pragma once
#include "base.h"
namespace glow
{
  template <typename PROPERTIES>
  struct ExchangeItem
  {
    uint8_t id;
    std::string name;
    std::string comment;
    void (*get)(PROPERTIES &p, char *value, size_t value_length);
    void (*set)(PROPERTIES &p, char *value);
  };
}
