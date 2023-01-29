#pragma once
#include <list>

#include "base.h"
#include "Properties.h"

namespace glow
{
  template <typename SPINNER>
  class Scene
  {
  private:
    std::forward_list<Properties> list;
    std::forward_list<SPINNER> layers;

  public:
    Properties &add_properties(Properties &properties)
    {
      auto iter = list.emplace_back();
      auto &emplaced = *iter;
      return emplaced.copy(properties);
    }
  };

} // namespace glow
