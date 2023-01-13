#pragma once

namespace glow
{
  struct Properties
  {
    float length = 0;
    float update_interval = 40.0;
    float grid_rows = 4.0;
    float hue_delta = -1.0;

    float gradient_hue = 0.0;
    float gradient_saturation = 255.0;
    float gradient_value = 255.0;

    float scan_hue = 85.0;
    float scan_saturation = 255.0;
    float scan_value = 255.0;
    float scan_width = 1.0;
  };
}