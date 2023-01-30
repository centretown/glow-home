#include "Properties.h"

namespace glow
{
#ifndef USE_ESP32
  ExchangeItem<Properties> Properties::exchange[PROPERTY_COUNT] = {
      {INTERVAL, "interval",
       "# update interval in ms",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.interval); },
       [](Properties &p, char *value)
       { sscanf(value, "%12u", &p.interval); }},

      {SCAN, "scan",
       "# don't scan=0, otherwise scan=n pixels",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.scan); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hu", &p.scan); }},

      {ORIGIN, "origin",
       "# top-left=0, top-right=1, bottom-left=2, bottom-right=3",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.origin); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hhu", &p.origin); }},

      {ORIENTATION, "orientation",
       "# horizontal=0, vertical=1, diagonal=2",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.orientation); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hhu", &p.orientation); }},

      {LENGTH, "length",
       "# number of pixel led's",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.length); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hu", &p.length); }},

      {ROWS, "rows",
       "# number of rows in grid",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.rows); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hu", &p.rows); }},

      {BEGIN, "begin",
       "# where to begin",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.begin); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hu", &p.begin); }},

      {END, "end",
       "# where to end",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.end); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hu", &p.end); }},

      {SOURCE, "source",
       "# gradient source: 0xVVSSHH (V-value, S-saturation, H-hue)",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "0x%x", hsv_to_u32(p.source)); },
       [](Properties &p, char *value)
       {
         uint32_t hsv_raw = 0;
         sscanf(value, "%12x", &hsv_raw);
         p.source = u32_to_hsv(hsv_raw);
       }},

      {TARGET, "target",
       "# gradient target: 0xVVSSHH (V-value, S-saturation, H-hue)",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "0x%x", hsv_to_u32(p.target)); },
       [](Properties &p, char *value)
       {
         uint32_t hsv_raw = 0;
         sscanf(value, "%12x", &hsv_raw);
         p.target = u32_to_hsv(hsv_raw);
       }},

      {SHIFT, "shift",
       "# amount to shift hue",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%d", p.shift); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hd", &p.shift); }},

      {TRANSFORM, "transform",
       "# transform",
       [](Properties &p, char *value, size_t value_length)
       { snprintf(value, value_length, "%u", p.transform); },
       [](Properties &p, char *value)
       { sscanf(value, "%12hhu", &p.transform); }},
  };

  std::unordered_map<std::string, uint8_t> Properties::property_map = {
      {exchange[INTERVAL].name, INTERVAL},
      {exchange[SCAN].name, SCAN},
      {exchange[ORIGIN].name, ORIGIN},
      {exchange[ORIENTATION].name, ORIENTATION},
      {exchange[LENGTH].name, LENGTH},
      {exchange[ROWS].name, ROWS},
      {exchange[BEGIN].name, BEGIN},
      {exchange[END].name, END},
      {exchange[SOURCE].name, SOURCE},
      {exchange[TARGET].name, TARGET},
      {exchange[SHIFT].name, SHIFT},
      {exchange[TRANSFORM].name, TRANSFORM},
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