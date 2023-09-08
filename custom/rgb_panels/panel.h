#pragma once

#include "esphome.h"

using esphome::display::BaseImage;
using esphome::rotary_encoder::RotaryEncoderResolution;

namespace panels
{
    constexpr uint32_t display_width = 128;
    constexpr uint32_t display_height = 64;

    constexpr uint32_t display_half_width = display_width >> 1;
    constexpr uint32_t display_half_height = display_height >> 1;
    constexpr uint32_t display_third_width = display_width / 3;
    constexpr uint32_t display_two_third_width = display_third_width << 1;

    constexpr uint32_t panel_width = 64;
    constexpr uint32_t panel_height = 32;

    enum PanelPos : uint32_t
    {
        TOP_LEFT,
        TOP_RIGHT,
        TOP_MIDDLE,
        BOTTOM_LEFT,
        BOTTOM_RIGHT,
        BOTTOM_MIDDLE,
    };

    struct Panel
    {
        // display panel position and dimensions
        int32_t x;
        int32_t y;
        int32_t width;
        int32_t height;
    };

}
