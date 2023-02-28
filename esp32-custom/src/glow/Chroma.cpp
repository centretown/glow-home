#include "Chroma.h"

namespace glow
{
#ifndef MICRO_CONTROLLER
  std::string Chroma::make_code()
  {
    std::stringstream s;
    s << "{" << length << ","
      << hsv_source.make_code() << ","
      << hsv_target.make_code() << ","
      << hue_shift << "}";
    return s.str();
  }

  std::string Chroma::keys[Chroma::KEY_COUNT] = {
      "length",
      "source",
      "target",
      "hue_shift",
  };
  Palette Chroma::palette{};
#endif

  // bool Chroma::setup(uint16_t p_length,
  //                    int16_t p_hue_shift,
  //                    Color p_source,
  //                    Color p_target)
  // {
  //   length = p_length;
  //   hue_shift = p_hue_shift;
  //   hsv_source.from_rgb(p_source);
  //   hsv_target.from_rgb(p_target);
  //   return setup();
  // }

  bool Chroma::setup(uint16_t p_length,
                     HSVColor p_source,
                     HSVColor p_target,
                     int16_t p_hue_shift)
  {
    length = p_length;
    hue_shift = p_hue_shift;
    hsv_source = p_source;
    hsv_target = p_target;
    return setup();
  }

  bool Chroma::setup(uint16_t p_length,
                     Color p_source,
                     HSVColor p_target,
                     int16_t p_hue_shift)
  {
    length = p_length;
    hue_shift = p_hue_shift;
    hsv_source.from_rgb(p_source);
    hsv_target = p_target;
    return setup();
  }

  bool Chroma::setup()
  {
    if (length == 0)
    {
      return false;
    }
    rgb_source = hsv_source.to_rgb();
    rgb_target = hsv_target.to_rgb();
    gradient_amount = (static_cast<float>(byte_limit) /
                       static_cast<float>(length)) /
                      static_cast<float>(byte_limit);
    return true;
  }
  
  void Chroma::update()
  {
    if (hue_shift == 0)
      return;

    auto update_hue = [&](HSVColor &hsv, Color &rgb)
    {
      hsv.hue += hue_shift;
      if (hsv.hue > hue_limit)
      {
        hsv.hue = (hue_shift < 0) ? hue_limit : 0;
      }
      rgb = hsv.to_rgb();
    };

    update_hue(hsv_source, rgb_source);
    update_hue(hsv_target, rgb_target);
  }

}