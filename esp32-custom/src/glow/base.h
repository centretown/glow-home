#pragma once

#ifdef USE_ESP32
#include "esphome.h"
#else
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <cstring>
#include <string>
#include <time.h>

#ifndef ALWAYS_INLINE
#define ALWAYS_INLINE __attribute__((always_inline))
#endif

const uint32_t clocks_sec = CLOCKS_PER_SEC;
const uint32_t clocks_ms = clocks_sec / 1000;
extern uint16_t millis_rate;
extern uint64_t millis_now;
extern bool real_time;
void set_real_time(bool v);
uint32_t millis32();
uint32_t micros32();

float gamma_correct(float value, float gamma);
float gamma_uncorrect(float value, float gamma);
void rgb_to_hsv(float red, float green, float blue,
                int &hue, float &saturation, float &value);
void hsv_to_rgb(int hue, float saturation, float value,
                float &red, float &green, float &blue);
#endif