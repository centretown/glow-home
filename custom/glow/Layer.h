#pragma once

#include <string>
#include <algorithm>

#include "base.h"

#ifndef MICRO_CONTROLLER
#include <yaml-cpp/yaml.h>
#include "Filer.h"
#endif

#include "Grid.h"
#include "Chroma.h"

namespace glow
{
  class Layer
  {
  private:
    uint16_t length = 0;
    uint16_t rows = 0;
    Grid grid;
    Chroma chroma;

    int16_t hue_shift = 0;
    uint16_t scan = 0;
    uint16_t begin = 0;
    uint16_t end = 100;

    // variant
    uint16_t position = 0;
    uint16_t first = 0;
    uint16_t last = 0;

  public:
    Layer() = default;

    Layer(uint16_t p_length,
          uint16_t p_rows,
          const Grid &p_grid,
          const Chroma &p_chroma,
          int16_t p_hue_shift = 0,
          uint16_t p_scan = 0,
          uint16_t p_begin = 0,
          uint16_t p_end = 100)
    {
      setup(p_length, p_rows, p_grid, p_chroma, p_hue_shift, p_scan, p_begin, p_end);
    }

    uint16_t get_length() const ALWAYS_INLINE { return length; }
    uint16_t get_rows() const ALWAYS_INLINE { return rows; }
    uint16_t get_begin() const ALWAYS_INLINE { return begin; }
    uint16_t get_end() const ALWAYS_INLINE { return end; }
    const Grid &get_grid() const ALWAYS_INLINE { return grid; }
    const Chroma get_chroma() const ALWAYS_INLINE { return chroma; }
    int16_t get_hue_shift() const ALWAYS_INLINE { return hue_shift; }
    uint16_t get_scan() const ALWAYS_INLINE { return scan; }
    uint16_t get_first() const ALWAYS_INLINE { return first; }
    uint16_t get_last() const ALWAYS_INLINE { return last; }

    bool setup()
    {
      if (length == 0)
      {
        return false;
      }

      if (rows == 0)
      {
        return false;
      }

      if (grid.setup_length(length, rows) == false)
      {
        return false;
      }

      if (chroma.setup_length(length, hue_shift) == false)
      {
        return false;
      }

      if (scan > length)
      {
        scan = length;
      }

      set_bounds();

      return true;
    }

    void set_bounds()
    {
      first = calculate_bounds(begin);
      last = calculate_bounds(end);

      if (last < first)
      {
        std::swap(first, last);
      }
    }

    uint16_t calculate_bounds(uint16_t bound);
    uint16_t adjust_bounds(uint16_t bound);

    bool setup(uint16_t p_length,
               uint16_t p_rows,
               const Grid &p_grid,
               const Chroma &p_chroma,
               int16_t p_hue_shift = 0,
               uint16_t p_scan = 0,
               uint16_t p_begin = 0,
               uint16_t p_end = 100)
    {
      length = p_length;
      rows = p_rows;
      grid = p_grid;
      chroma = p_chroma;
      hue_shift = p_hue_shift;
      scan = p_scan;
      begin = p_begin;
      end = p_end;
      return setup();
    }

    bool setup_length(uint16_t a_length, uint16_t a_rows = 1) ALWAYS_INLINE
    {
      length = a_length;
      rows = a_rows;
      return setup();
    }

    void update_position(uint16_t &start_at, uint16_t &end_at) ALWAYS_INLINE
    {
      start_at = position;
      end_at = position + scan;

      position++;
      if (position >= last)
      {
        position = first;
      }
    }

    template <typename LIGHT>
    void spin(LIGHT &light)
    {
      uint16_t start_at{first};
      uint16_t end_at{last};

      if (scan > 0)
      {
        update_position(start_at, end_at);
      }

      for (uint16_t i = start_at; i < end_at; ++i)
      {
        light.get(grid.map(i)) = chroma.map(i).get();
      }
      chroma.update();
    }

#ifndef MICRO_CONTROLLER
    enum : uint8_t
    {
      LENGTH,
      ROWS,
      GRID,
      CHROMA,
      HUE_SHIFT,
      SCAN,
      BEGIN,
      END,
      KEY_COUNT,
    };

    static std::string keys[KEY_COUNT];
    friend YAML::convert<Layer>;
    std::string make_code();
#endif
  };
} // namespace glow

#ifndef MICRO_CONTROLLER
namespace YAML
{
  using namespace glow;

  template <>
  struct convert<Layer>
  {
    static void lookup(Node item, Chroma &chroma)
    {
      if (item.IsMap())
      {
        chroma = item.as<Chroma>();
        return;
      }

      std::string name = item.as<std::string>();
      if (load_yaml(custom_chroma(name), chroma))
      {
        return;
      }
      return;
    }

    static void lookup(Node item, Grid &grid)
    {
      if (item.IsMap())
      {
        grid = item.as<Grid>();
        return;
      }

      std::string name = item.as<std::string>();
      if (load_yaml(custom_grid(name), grid))
      {
        return;
      }
      return;
    }

    static Node encode(const Layer &layer)
    {
      Node node;
      node[Layer::keys[Layer::LENGTH]] = layer.length;
      node[Layer::keys[Layer::ROWS]] = layer.rows;
      node[Layer::keys[Layer::GRID]] = layer.grid;
      node[Layer::keys[Layer::CHROMA]] = layer.chroma;
      node[Layer::keys[Layer::HUE_SHIFT]] = layer.hue_shift;
      node[Layer::keys[Layer::SCAN]] = layer.scan;
      node[Layer::keys[Layer::BEGIN]] = layer.begin;
      node[Layer::keys[Layer::END]] = layer.end;
      return node;
    }

    static bool decode(const Node &node, Layer &layer)
    {
      if (!node.IsMap())
      {
        layer.setup();
        return false;
      }

      for (auto key = 0; key < Layer::KEY_COUNT; ++key)
      {
        Node item = node[Layer::keys[key]];
        if (!item.IsDefined())
        {
          continue;
        }

        switch (key)
        {
        case Layer::LENGTH:
          layer.length = item.as<uint16_t>();
          break;
        case Layer::ROWS:
          layer.rows = item.as<uint16_t>();
          break;
        case Layer::GRID:
          lookup(item, layer.grid);
          break;
        case Layer::CHROMA:
          lookup(item, layer.chroma);
          break;
        case Layer::HUE_SHIFT:
          layer.hue_shift = item.as<int16_t>();
          break;
        case Layer::SCAN:
          layer.scan = item.as<uint16_t>();
          break;
        case Layer::BEGIN:
          layer.begin = item.as<uint16_t>();
          break;
        case Layer::END:
          layer.end = item.as<uint16_t>();
          break;
        }
      }

      layer.setup();
      return true;
    }
  };
}
#endif