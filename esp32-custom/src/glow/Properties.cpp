#include "Properties.h"

namespace glow
{
  std::string Properties::property_names[] = {
      "current_color",
      "origin",
      "orientation",
      "length",
      "color_transform",
      "grid_rows",
      "hue_delta",
      "gradient_hue",
      "gradient_saturation",
      "gradient_value",
      "scan_width",
  };

  std::unordered_map<std::string, uint8_t> Properties::property_map = {
      {property_names[CURRENT_COLOR], CURRENT_COLOR},
      {property_names[ORIGIN], ORIGIN},
      {property_names[ORIENTATION], ORIENTATION},
      {property_names[LENGTH], LENGTH},
      {property_names[COLOR_TRANSFORM], COLOR_TRANSFORM},
      {property_names[GRID_ROWS], GRID_ROWS},
      {property_names[HUE_DELTA], HUE_DELTA},
      {property_names[GRADIENT_HUE], GRADIENT_HUE},
      {property_names[GRADIENT_SATURATION], GRADIENT_SATURATION},
      {property_names[GRADIENT_VALUE], GRADIENT_VALUE},
      {property_names[SCAN_WIDTH], SCAN_WIDTH},
  };

  void Properties::set(const char *key, char *value)
  {
    auto iter = property_map.find(key);
    auto val = *iter;
    set(val.second, value);
  }

  void Properties::get(const char *key, char *value, size_t value_length)
  {
    auto iter = property_map.find(key);
    auto val = *iter;
    get(val.second, value, value_length);
  }

  void Properties::set(uint8_t key, char *value)
  {
    switch (key)
    {
    case CURRENT_COLOR:
      sscanf(value, "%16u", &current_color.raw_32);
      break;
    case ORIGIN:
      sscanf(value, "%16hhu", &origin);
      break;
    case ORIENTATION:
      sscanf(value, "%16hhu", &orientation);
      break;
    case LENGTH:
      sscanf(value, "%16hu", &length);
      break;
    case COLOR_TRANSFORM:
      break;
    case GRID_ROWS:
      sscanf(value, "%16f", &grid_rows);
      break;
    case HUE_DELTA:
      sscanf(value, "%16f", &hue_delta);
      break;
    case GRADIENT_HUE:
      sscanf(value, "%16f", &gradient_hue);
      break;
    case GRADIENT_SATURATION:
      sscanf(value, "%16f", &gradient_saturation);
      break;
    case GRADIENT_VALUE:
      sscanf(value, "%16f", &gradient_value);
      break;
    case SCAN_WIDTH:
      sscanf(value, "%16f", &scan_width);
      break;
    default:
      break;
    }
  }

  void Properties::get(uint8_t key, char *value, size_t value_length)
  {
    switch (key)
    {
    case CURRENT_COLOR:
      snprintf(value, value_length, "%u", current_color.raw_32);
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
    case COLOR_TRANSFORM:
      snprintf(value, value_length, "%u", color_transform);
      break;
    case GRID_ROWS:
      snprintf(value, value_length, "%.1f", grid_rows);
      break;
    case HUE_DELTA:
      snprintf(value, value_length, "%.1f", hue_delta);
      break;
    case GRADIENT_HUE:
      snprintf(value, value_length, "%.1f", gradient_hue);
      break;
    case GRADIENT_SATURATION:
      snprintf(value, value_length, "%.1f", gradient_saturation);
      break;
    case GRADIENT_VALUE:
      snprintf(value, value_length, "%.1f", gradient_value);
      break;
    case SCAN_WIDTH:
      snprintf(value, value_length, "%.1f", scan_width);
      break;
    default:
      snprintf(value, value_length, "-999.99");
      break;
    }
  }
}