#include "Properties.h"

namespace glow
{
#ifndef USE_ESP32
  std::string Properties::property_names[] = {
      "interval",
      "scan",
      "origin",
      "orientation",
      "length",
      "rows",
      "begin",
      "end",
      "source",
      "target",
      "shift",
      "transform",
  };

  std::unordered_map<std::string, uint8_t> Properties::property_map = {
      {property_names[INTERVAL], INTERVAL},
      {property_names[SCAN], SCAN},
      {property_names[ORIGIN], ORIGIN},
      {property_names[ORIENTATION], ORIENTATION},
      {property_names[LENGTH], LENGTH},
      {property_names[ROWS], ROWS},
      {property_names[BEGIN], BEGIN},
      {property_names[END], END},
      {property_names[SOURCE], SOURCE},
      {property_names[TARGET], TARGET},
      {property_names[SHIFT], SHIFT},
      {property_names[TRANSFORM], TRANSFORM},
  };

  PropertyItem Properties::io_items[PROPERTY_COUNT] = {
      {"# update interval in ms",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.interval); },
       [](Properties &p, char *value)
       { sscanf(value, "%12u", &p.interval); }},

      {"# don't scan=0, otherwise scan=n pixels",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.scan); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hu", &p.scan); }},

      {"# top-left=0, top-right=1, bottom-left=2, bottom-right=3",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.origin); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hhu", &p.origin); }},

      {"# horizontal=0, vertical=1, diagonal=2",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.orientation); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hhu", &p.orientation); }},

      {"# number of pixel led's",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.length); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hu", &p.length); }},

      {"# number of rows in grid",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.rows); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hu", &p.rows); }},

      {"# where to begin",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.begin); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hu", &p.begin); }},

      {"# where to end",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.end); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hu", &p.end); }},

      {"# gradient source: 0xVVSSHH (V-value, S-saturation, H-hue)",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "0x%x", p.hsv_to_u32(p.source)); },
       [](Properties &p, char *value)
       {
         uint32_t hsv_raw = 0;
         sscanf(value, "%12x", &hsv_raw);
         p.source = p.u32_to_hsv(hsv_raw);
       }},

      {"# gradient target: 0xVVSSHH (V-value, S-saturation, H-hue)",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "0x%x", p.hsv_to_u32(p.target)); },
       [](Properties &p, char *value)
       {
         uint32_t hsv_raw = 0;
         sscanf(value, "%12x", &hsv_raw);
         p.target = p.u32_to_hsv(hsv_raw);
       }},

      {"# amount to shift hue",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%d", p.shift); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hd", &p.shift); }},

      {"# transform",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.transform); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hhu", &p.transform); }},
  };

  bool Properties::set(const char *key, char *value)
  {
    auto iter = property_map.find(key);
    if (iter == property_map.end())
    {
      return false;
    }
    auto val = *iter;
    // set(val.second, value);
    io_items[val.second].set(*this, value);
    return true;
  }

  bool Properties::get(const char *key, char *value, size_t value_length)
  {
    auto iter = property_map.find(key);
    if (iter == property_map.end())
    {
      return false;
    }
    auto val = *iter;
    io_items[val.second].get(*this, value, value_length);
    // get(val.second, value, value_length);
    return true;
  }

#endif

  Properties &Properties::copy(Properties &p)
  {
    interval = p.interval;
    scan = p.scan;
    origin = p.origin;
    orientation = p.orientation;
    length = p.length;
    rows = p.rows;
    begin = p.begin;
    end = p.end;
    source = p.source;
    target = p.target;
    shift = p.shift;
    transform = p.transform;
    return *this;
  }

  constexpr uint16_t byte_limit = 0xff;
  constexpr uint16_t hue_limit = 1530;
  constexpr uint16_t hue_segment = hue_limit / 6;

  constexpr uint16_t hue_red = 0;
  // constexpr uint16_t hue_yellow = hue_segment;
  constexpr uint16_t hue_green = hue_limit / 3;
  // constexpr uint16_t hue_cyan = hue_limit / 2;
  constexpr uint16_t hue_blue = hue_limit * 2 / 3;
  // constexpr uint16_t hue_magenta = hue_limit * 5 / 6;

  ESPHSVColor Properties::color_to_hsv(Color color)
  {
    const uint16_t red = color.red;
    const uint16_t green = color.green;
    const uint16_t blue = color.blue;

    const uint16_t value = std::max(red, std::max(green, blue));
    const uint16_t color_range = value -
                                 std::min(red, std::min(green, blue));
    const uint16_t saturation =
        (value == 0) ? 0
                     : (color_range * byte_limit) / value;
    uint16_t hue = 0;
    if (color_range != 0)
    {
      if (value == red)
      {
        hue = (hue_segment * (green - blue) / color_range) + hue_limit;
      }
      else if (value == green)
      {
        hue = (hue_segment * (blue - red) / color_range) + hue_green;
      }
      else // if (value == blue)
      {
        hue = (hue_segment * (red - green) / color_range) + hue_blue;
      }
    }

    return ESPHSVColor(static_cast<uint8_t>(hue / 6),
                       static_cast<uint8_t>(saturation),
                       static_cast<uint8_t>(value));
  }

#ifndef USE_ESP32
  // keep original for testing
  ESPHSVColor Properties::old_color_to_hsv(Color color)
  {
    float red = static_cast<float>(color.red) / 255.0;
    float green = static_cast<float>(color.green) / 255.0;
    float blue = static_cast<float>(color.blue) / 255.0;
    float saturation, value;
    int hue;

    rgb_to_hsv(red, green, blue, hue, saturation, value);

    hue *= 255;
    hue /= 360;
    saturation *= 255;
    value *= 255;
    return ESPHSVColor(static_cast<uint8_t>(hue),
                       static_cast<uint8_t>(saturation),
                       static_cast<uint8_t>(value));
  }
#endif

}