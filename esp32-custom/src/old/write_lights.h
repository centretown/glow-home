// Copyright (c) 2022 Dave Marsh. See LICENSE.

#pragma once

#include "esphome.h"
using namespace esphome;
using namespace light;

void hsvValue(AddressableLight &light, int &hue, bool initial_run);
void hsvSaturation(AddressableLight &light, int &hue, bool initial_run);
ESPHSVColor current_hsv_color(Color &current_color);
