#pragma once
#include <list>

#include "base.h"
#ifndef MICRO_CONTROLLER
#include <yaml-cpp/yaml.h>
#endif

#ifdef ESPHOME_CONTROLLER
#include "esphome.h"
#endif

#include "Layer.h"

namespace glow
{
  class Frame
  {
  private:
    uint16_t length = 0;
    uint16_t rows = 0;
    uint32_t interval = 16;
    uint32_t next = 0;

  public:
    std::list<Layer> layers;

  public:
    Frame() = default;

    Frame(uint16_t p_length,
          uint16_t p_rows,
          uint32_t p_interval,
          std::initializer_list<Layer> p_layers)
    {
      length = p_length;
      rows = p_rows;
      interval = p_interval;
      layers = p_layers;
    }

    Frame(const Frame &frame)
    {
      copy(frame);
    }

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

      for (auto &layer : layers)
      {
        layer.setup_length(length, rows);
      }

      return true;
    }

    bool setup(uint16_t p_length, uint16_t p_rows, uint32_t p_interval)
    {
      length = p_length;
      rows = p_rows;
      interval = p_interval;
      return setup();
    }

    void copy(const Frame &frame);

    template <typename LIGHT>
    void spin(LIGHT *light)
    {
      for (auto &layer : layers)
      {
        layer.spin(light);
      }
#ifndef ESPHOME_CONTROLLER
      light->update();
#endif
    }

    uint16_t get_length() const ALWAYS_INLINE { return length; }
    uint16_t get_rows() const ALWAYS_INLINE { return rows; }
    uint32_t get_interval() const ALWAYS_INLINE { return interval; }

    size_t get_size() const ALWAYS_INLINE { return layers.size(); }
    std::list<Layer>::const_iterator begin() const ALWAYS_INLINE { return layers.begin(); }
    std::list<Layer>::const_iterator end() const ALWAYS_INLINE { return layers.end(); }
    void push_back(Layer layer) ALWAYS_INLINE { layers.push_back(layer); }

#ifdef ESPHOME_CONTROLLER
    bool is_ready() ALWAYS_INLINE
    {
#define millis() esphome::millis()
      const uint32_t now = millis();

      if (next - now > interval)
      {
        next = now + interval;
        return true;
      }
      return false;
    }
#endif

#ifndef MICRO_CONTROLLER
    enum : uint8_t
    {
      LENGTH,
      ROWS,
      INTERVAL,
      LAYERS,
      KEY_COUNT,
    };
    static std::string keys[KEY_COUNT];
    friend YAML::convert<Frame>;
    std::string make_code();
#endif
  };
} // namespace glow

#ifndef MICRO_CONTROLLER
namespace YAML
{
  using namespace glow;

  template <>
  struct convert<Frame>
  {
    static void lookup(Node item, Layer &layer)
    {
      if (item.IsMap())
      {
        layer = item.as<Layer>();
        return;
      }

      std::string name = item.as<std::string>();
      if (load_yaml(custom_layer(name), layer))
      {
        return;
      }
      return;
    }

    static Node encode(const Frame &frame)
    {
      Node node;
      node[Frame::keys[Frame::LENGTH]] = frame.length;
      node[Layer::keys[Frame::ROWS]] = frame.rows;
      node[Frame::keys[Frame::INTERVAL]] = frame.interval;
      Node list;
      for (auto layer : frame.layers)
      {
        list.push_back(layer);
      }
      node[Frame::keys[Frame::LAYERS]] = list;
      return node;
    }

    static bool decode(const Node &node, Frame &frame)
    {
      if (!node.IsMap())
      {
        frame.setup();
        return false;
      }

      for (auto key = 0; key < Frame::KEY_COUNT; ++key)
      {
        Node item = node[Frame::keys[key]];
        if (!item.IsDefined())
        {
          continue;
        }

        switch (key)
        {
        case Frame::LENGTH:
          frame.length = item.as<uint16_t>();
          break;
        case Frame::ROWS:
          frame.rows = item.as<uint16_t>();
          break;
        case Frame::INTERVAL:
          frame.interval = item.as<uint32_t>();
          break;
        case Frame::LAYERS:
          if (item.IsSequence())
          {
            Layer layer;
            for (auto layer_node : item)
            {
              lookup(layer_node, layer);
              frame.push_back(layer);
            }
          }
          break;
        }
      }

      frame.setup();
      return true;
    }
  };
}
#endif