#pragma once

#ifndef ALWAYS_INLINE
#define ALWAYS_INLINE __attribute__((always_inline))
#endif

#include <cstdlib>

// #ifdef USE_ESP32
#define MICRO_CONTROLLER
#define ESPHOME_CONTROLLER
// #endif