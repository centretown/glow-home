#pragma once

#include "esphome.h"

namespace panels
{
    struct RotaryState
    {
        esphome::rotary_encoder::RotaryEncoderResolution mode;
        int32_t min_value;
        int32_t max_value;
        int32_t value;
        bool wrap;
    };
}