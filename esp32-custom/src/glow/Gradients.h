#pragma once

#include "esphome.h"
using namespace esphome;
using namespace light;

#include "Grid.h"
#include "GradientRange.h"
#include "Pixels.h"

namespace glow
{
  class Gradients
  {
  public:
    ESPHSVColor hsv;

  private:
    Pixels pixels;
    Grid grid;

    GradientRange hue_range = {0, 1};
    GradientRange saturation_range = {255, 256};
    GradientRange luminance_range = {255, 256};

    bool is_setup = false;
    bool is_logged = false;

  private:
  public:
    void Setup(AddressableLight *it, Color color, uint16_t row_count = 1)
    {
      pixels.setup(it);
      grid.Resize(0, pixels.size(), row_count);
      if (grid.Rows() > 1)
      {
        grid.Profile(GRID_COLUMNS);
      }

      hsv = hsv_color(color);
      hue_range((uint16_t)hsv.hue,
                (uint16_t)hsv.hue + hue_range.Length());

      hsv.saturation = saturation_range.Begin();
      hsv.value = luminance_range.Begin();
      is_setup = true;
    }

    inline void asColumns(uint16_t row_count = out_of_bounds)
    {
      if (row_count != out_of_bounds)
      {
        grid.Resize(0, pixels.size(), row_count);
      }
      grid.Profile(GRID_COLUMNS);
    }

    inline void asRows()
    {
      grid.Profile(GRID_ROWS);
    }

    inline void Run(bool is_reversed = false)
    {
      if (is_setup == false)
      {
        log_not_setup();
        return;
      }

      if (is_logged == false)
      {
        log_setup();
      }

      grid.Reversed(is_reversed);
      grid.SpinValues(pixels, grid, *this);
    }

    //   inline ESPHSVColor Map(uint16_t index)
    //   {
    //     hsv.saturation = saturation_range.get(grid, index);
    //     hsv.value = luminance_range.get(grid, index);
    //     hsv.hue = hue_range.get(grid, index);
    //     return hsv;
    //   }

    // private:
    inline ESPHSVColor Map(uint16_t index)
    {
      float ratio = static_cast<float>(index) - grid.Begin();
      ratio /= static_cast<float>(grid.Length());

      hsv.hue = hue_range.adjust(ratio);
      hsv.saturation = saturation_range.adjust(ratio);
      hsv.value = luminance_range.adjust(ratio);
      return hsv;
    }

  public:
    inline void Hue(uint16_t from, uint16_t to = out_of_bounds)
    {
      hue_range.set(from, to);
    }
    inline void Saturation(uint16_t from, uint16_t to = out_of_bounds)
    {
      saturation_range.set(from, to);
    }
    inline void Luminance(uint16_t from, uint16_t to = out_of_bounds)
    {
      luminance_range.set(from, to);
    }

    inline void shiftHue(uint8_t amount)
    {
      hue_range.shift(amount);
    }

    void log_setup()
    {
      ESP_LOGD("glow-Gradients", "size: %d range from %u to %u",
               pixels.size(), (uint)grid.Begin(), (uint)grid.End());
      ESP_LOGD("glow-Gradients", "hue h:%u-%u-%u sat:%u-%u-%u val:%u-%u-%u",
               hue_range.Begin(), hue_range.End(), hue_range.Reversed(),
               saturation_range.Begin(), saturation_range.End(), saturation_range.Reversed(),
               luminance_range.Begin(), luminance_range.End(), luminance_range.Reversed());

      is_logged = true;
    }

    void log_not_setup()
    {
      if (!is_logged)
      {
        ESP_LOGD("glow-Gradients", "Not Setup!");
        is_logged = true;
      }
    }

    inline bool FlipHue(uint32_t &counter, bool &reverse,
                        uint16_t lo, uint16_t hi,
                        uint32_t mlls, uint32_t gap = 5000)
    {
      return Flip(hue_range, counter, reverse, lo, hi, mlls, gap);
    }

    inline bool FlipSaturation(uint32_t &counter, bool &reverse,
                               uint16_t lo, uint16_t hi,
                               uint32_t mlls, uint32_t gap = 5000)
    {
      return Flip(saturation_range, counter, reverse, lo, hi, mlls, gap);
    }

    inline bool FlipLuminance(uint32_t &counter, bool &reverse,
                              uint16_t lo, uint16_t hi,
                              uint32_t mlls, uint32_t gap = 5000)
    {
      return Flip(luminance_range, counter, reverse, lo, hi, mlls, gap);
    }

    inline bool Flip(GradientRange &gradient, uint32_t &counter, bool &reverse,
                     uint16_t lo, uint16_t hi,
                     uint32_t mlls, uint32_t gap = 5000)
    {
      if (mlls >= counter)
      {
        counter = mlls + gap;
        reverse = !reverse;
        if (reverse)
        {
          gradient(hi, lo);
        }
        else
        {
          gradient(lo, hi);
        }
        return true;
      }
      return false;
    }
  };

}