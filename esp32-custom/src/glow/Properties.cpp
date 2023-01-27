#include "Properties.h"

namespace glow
{
  std::string Properties::property_names[] = {
      "interval",
      "scan_width",
      "origin",
      "orientation",
      "length",
      "rows",
      "source",
      "target",
      "shift",
      "transform",
  };

  std::unordered_map<std::string, uint8_t> Properties::property_map = {
      {property_names[INTERVAL], INTERVAL},
      {property_names[SCAN_WIDTH], SCAN_WIDTH},
      {property_names[ORIGIN], ORIGIN},
      {property_names[ORIENTATION], ORIENTATION},
      {property_names[LENGTH], LENGTH},
      {property_names[ROWS], ROWS},
      {property_names[SOURCE], SOURCE},
      {property_names[TARGET], TARGET},
      {property_names[SHIFT], SHIFT},
      {property_names[TRANSFORM], TRANSFORM},
  };

  bool Properties::set(const char *key, char *value)
  {
    auto iter = property_map.find(key);
    if (iter == property_map.end())
    {
      return false;
    }
    auto val = *iter;
    set(val.second, value);
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
    get(val.second, value, value_length);
    return true;
  }

  void Properties::set(uint8_t key, char *value)
  {
    uint32_t hsv_raw = 0;
    switch (key)
    {
    case INTERVAL:
      sscanf(value, "%8u", &interval);
      break;
    case SCAN_WIDTH:
      sscanf(value, "%8hu", &scan_width);
      break;
    case ORIGIN:
      sscanf(value, "%8hhu", &origin);
      break;
    case ORIENTATION:
      sscanf(value, "%8hhu", &orientation);
      break;
    case LENGTH:
      sscanf(value, "%8hu", &length);
      break;
    case ROWS:
      sscanf(value, "%8hu", &rows);
      break;
    case SOURCE:
      sscanf(value, "%8u", &hsv_raw);
      source = u32_to_hsv(hsv_raw);
      break;
    case TARGET:
      sscanf(value, "%8u", &hsv_raw);
      target = u32_to_hsv(hsv_raw);
      break;
    case SHIFT:
      sscanf(value, "%8hd", &shift);
      break;
    case TRANSFORM:
      break;
    default:
      break;
    }
  }

  void Properties::get(uint8_t key, char *value, size_t value_length)
  {
    switch (key)
    {
    case INTERVAL:
      snprintf(value, value_length, "%u", interval);
      break;
    case SCAN_WIDTH:
      snprintf(value, value_length, "%u", scan_width);
      break;
    case ORIGIN:
      snprintf(value, value_length, "%u", origin);
      break;
    case ORIENTATION:
      snprintf(value, value_length, "%u", orientation);
      break;
    case LENGTH:
      snprintf(value, value_length, "%u", length);
      break;
    case ROWS:
      snprintf(value, value_length, "%u", rows);
      break;
    case SOURCE:
      snprintf(value, value_length, "%u", hsv_to_u32(source));
      break;
    case TARGET:
      snprintf(value, value_length, "%u", hsv_to_u32(target));
      break;
    case SHIFT:
      snprintf(value, value_length, "%d", shift);
      break;
    case TRANSFORM:
      snprintf(value, value_length, "%u", transform);
      break;
    }
  }

  constexpr uint16_t byte_limit = 0xff;
  constexpr uint16_t hue_limit = 1530;
  constexpr uint16_t hue_segment = hue_limit / 6;

  constexpr uint16_t hue_red = 0;
  constexpr uint16_t hue_yellow = hue_segment;
  constexpr uint16_t hue_green = hue_limit / 3;
  constexpr uint16_t hue_cyan = hue_limit / 2;
  constexpr uint16_t hue_blue = hue_limit * 2 / 3;
  constexpr uint16_t hue_magenta = hue_limit * 5 / 6;

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