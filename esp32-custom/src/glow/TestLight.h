#pragma once

namespace glow
{
  class TestLight
  {
  private:
    Color dummy;

  public:
    Color &get(uint16_t)
    {
      return dummy;
    }
  };

} // namespace glow
