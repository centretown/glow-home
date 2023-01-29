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

  ExchangeItem<Properties> Properties::exchange[PROPERTY_COUNT] = {
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
       { snprintf(value, value_length, "0x%x", hsv_to_u32(p.source)); },
       [](Properties &p, char *value)
       {
         uint32_t hsv_raw = 0;
         sscanf(value, "%12x", &hsv_raw);
         p.source = u32_to_hsv(hsv_raw);
       }},

      {"# gradient target: 0xVVSSHH (V-value, S-saturation, H-hue)",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "0x%x", hsv_to_u32(p.target)); },
       [](Properties &p, char *value)
       {
         uint32_t hsv_raw = 0;
         sscanf(value, "%12x", &hsv_raw);
         p.target = u32_to_hsv(hsv_raw);
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
    exchange[val.second].set(*this, value);
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
    exchange[val.second].get(*this, value, value_length);
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

  
}