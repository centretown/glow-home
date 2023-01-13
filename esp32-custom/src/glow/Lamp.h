#pragma once

#include "esphome.h"
using namespace esphome;
using namespace light;

#include "Properties.h"
#include "Grid.h"
#include "Chroma.h"
#include "Scanner.h"
#include "color_to_hsv.h"

extern template_::TemplateNumber *update_interval;
extern template_::TemplateNumber *grid_rows;
extern template_::TemplateNumber *hue_delta;

extern template_::TemplateNumber *gradient_hue;
extern template_::TemplateNumber *gradient_saturation;
extern template_::TemplateNumber *gradient_value;

extern template_::TemplateNumber *scan_hue;
extern template_::TemplateNumber *scan_saturation;
extern template_::TemplateNumber *scan_value;
extern template_::TemplateNumber *scan_width;

namespace glow
{
  class Lamp
  {
  protected:
    AddressableLight *light = nullptr;

    Grid grid;
    Chroma chroma;
    Scanner scanner;

    uint32_t next = 0;
    uint32_t interval = 48;

    bool is_setup = false;

  public:
    void setup(AddressableLight *it,
               Color current_color,
               Origin origin = TopLeft,
               Orientation orientation = Horizontal)
    {
      light = it;
      if (light == nullptr)
      {
        return;
      }

      interval = static_cast<uint32_t>(update_interval->state);

      grid.setup((uint16_t)light->size(),
                 static_cast<uint16_t>(grid_rows->state),
                 origin, orientation);

      chroma.setup(current_color,
                   ESPHSVColor(static_cast<uint8_t>(gradient_hue->state),
                               static_cast<uint8_t>(gradient_saturation->state),
                               static_cast<uint8_t>(gradient_value->state)),
                   static_cast<int16_t>(hue_delta->state));

      auto scan_color = ESPHSVColor(static_cast<uint8_t>(scan_hue->state),
                                    static_cast<uint8_t>(scan_saturation->state),
                                    static_cast<uint8_t>(scan_value->state));
      scanner.setup(scan_width->state);
    }

    void setup(Properties &properties,
               AddressableLight *it,
               Color current_color,
               Origin origin = TopLeft,
               Orientation orientation = Horizontal)
    {
      light = it;
      if (light == nullptr)
      {
        return;
      }

      interval = static_cast<uint32_t>(properties.update_interval);

      grid.setup(static_cast<uint16_t>(properties.length),
                 static_cast<uint16_t>(grid_rows->state),
                 origin, orientation);

      chroma.setup(current_color,
                   ESPHSVColor(static_cast<uint8_t>(properties.gradient_hue),
                               static_cast<uint8_t>(properties.gradient_saturation),
                               static_cast<uint8_t>(properties.gradient_value)),
                   static_cast<int16_t>(properties.hue_delta));

      scanner.setup(scan_width->state);
      is_setup = 1;
      log();
    }

    bool is_ready() ALWAYS_INLINE
    {
#define millis() esphome::millis()
      const uint32_t now = millis();

      if (next - now > interval && is_setup)
      {
        next = now + interval;
        return true;
      }
      return false;
    }

    void update_hue() ALWAYS_INLINE
    {
      chroma.update_hue();
    }

    virtual void log() const
    {
      char buffer[80];
      grid.log_buffer(buffer, sizeof(buffer));

      ESP_LOGD("glow-Lamp", buffer);

      ESP_LOGD("glow-Lamp",
               "interval=%u, delta=%d",
               interval, delta);
      ESP_LOGD("glow-Lamp",
               "color=red:%u green:%u blue:%u",
               color.red, color.green, color.blue);
      ESP_LOGD("glow-Lamp",
               "hsv_color=hue:%u saturation:%u value:%u",
               hsv_color.hue, hsv_color.saturation,
               hsv_color.value);
      ESP_LOGD("glow-Lamp",
               "is_setup=%u, is_logged=%u",
               is_setup, is_logged);
    }

    uint16_t map_columns(uint16_t i, div_t &point) ALWAYS_INLINE
    {
      return grid.map_columns(i, point);
    }

    uint16_t map_diagonal(uint16_t index)
    {
      return grid.map_diagonal(index);
    }

    template <typename COLOR_MAPPER>
    void spin(uint16_t begin, uint16_t end,
              COLOR_MAPPER &color_mapper)
    {
      for (uint16_t i = begin; i < end; ++i)
      {
        light->get(i) = color_mapper(i);
      }
      update_hue();
    }

    template <typename MAPPER, typename COLOR_MAPPER>
    void spin(uint16_t begin, uint16_t end,
              MAPPER &mapper, COLOR_MAPPER &color_mapper)
    {
      for (uint16_t i = begin; i < end; ++i)
      {
        uint16_t offset = mapper(i);
        light->get(offset) = color_mapper(i);
      }
      update_hue();
    }
  };
}
