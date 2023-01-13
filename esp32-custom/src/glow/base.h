#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <cstring>
#include <string>

#ifndef ALWAYS_INLINE
#define ALWAYS_INLINE __attribute__((always_inline))
#endif

float gamma_correct(float value, float gamma);
float gamma_uncorrect(float value, float gamma);
void rgb_to_hsv(float red, float green, float blue,
                int &hue, float &saturation, float &value);
void hsv_to_rgb(int hue, float saturation, float value,
                float &red, float &green, float &blue);
