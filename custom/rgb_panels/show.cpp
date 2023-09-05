
#include "control.h"
#include <cmath>
#include <string.h>
#include <algorithm>

namespace rgb_panels
{
  constexpr int padding_x = 3;
  constexpr int padding_y = 2;
  constexpr float MY_PI = 3.1415926535897932384626433832795;

  Resources resources = {
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL,
      {
          NULL,
      },
  };

  esphome::display::BaseImage *light_bulb(int brightness)
  {
    return resources.bulbs[(brightness / 10) % 11];
  }

  esphome::text_sensor::TextSensor *Controller::wifi_info[INFO_COUNT] = {
      NULL,
  };

  void draw_guage(esphome::display::Display &display, int value, int range)
  {
    const int guage_max = display_width - (padding_x << 1);
    const int guage_height = display_half_height - (padding_y << 1);
    display.rectangle(padding_x, padding_y, guage_max, guage_height);
    const int guage_length = (value * guage_max) / range;
    display.filled_rectangle(padding_x, padding_y, guage_length, guage_height);
  }

  void draw_bulbs(esphome::display::Display &display,
                  Resources &resources, int x, int y, int value)
  {
    char buffer[8];
    snprintf(buffer, sizeof(buffer), "%3d", value);
    int width, x_offset, baseline, line_height;
    resources.large->measure(buffer, &width, &x_offset, &baseline, &line_height);
    display.print(x - width - padding_x, y, resources.large, buffer);
    display.image(x + padding_x, y, light_bulb(value));
  }

  void Controller::setup(Resources *res)
  {
    resources.small = res->small;
    resources.medium = res->medium;
    resources.large = res->large;
    resources.up = res->up;
    resources.down = res->down;
    resources.effect = res->effect;
    const auto count = sizeof(resources.bulbs) / sizeof(resources.bulbs[0]);
    for (int i = 0; i < count; i++)
    {
      resources.bulbs[i] = res->bulbs[i];
    }
  };

  Panel edit_panel =
      {0, 0, display_width, display_height};

  const std::vector<float> wifi_signals = {
      -44.0, -67.0, -70.0, -80.0};

  int Controller::wifi_bar_count(float signal)
  {
    if (std::isnan(signal))
      return 0;

    int bars = wifi_signals.size();
    for (auto s : wifi_signals)
    {
      if (signal > s)
      {
        return bars;
      }
      --bars;
    }

    return 0;
  }

  void Controller::show(esphome::display::Display &display)
  {
    if (is_edit)
      edit(display);
    else
      show_all(display);
  }

  void Controller::show_all(esphome::display::Display &display)
  {
    if (panel_id < LIGHT_RED)
    {
      show_brightness(display);
      show_effect(display);
      show_wifi_signal(display);
    }
    else if (panel_id < LIGHT_HUE)
    {
      show_colors(display);
    }
    else if (panel_id < PANEL_COUNT)
    {
      show_colors_hsv(display);
    }
    else
    {
      return;
    }
    show_border(display, panels[panel_id]);
  }

  void Controller::edit(esphome::display::Display &display)
  {
    if (panel_id == LIGHT_EFFECT)
    {
      edit_effect(display);
      return;
    }
    if (panel_id == WIFI_SIGNAL)
    {
      edit_wifi_signal(display);
      return;
    }

    if (panel_id < LIGHT_HUE)
      edit_gauge(display, rotary_settings[panel_id]);
    else if (panel_id < PANEL_COUNT)
      show_hsv(display);
  }

  void Controller::show_wifi_signal(esphome::display::Display &display)
  {
    Panel &panel = panels[WIFI_SIGNAL];

    int signal = wifi_signal;
    signal = std::min(std::max(2 * (signal + 100), 0), 100);
    display.printf(panel.x + padding_x, panel.y + padding_y,
                   resources.medium, "%d", signal);

    const int bar_count = 5;
    const int bar_width = 6;
    const int bar_space = 1;
    const int bar_begin = panel_width - padding_x - bar_count * (bar_width + bar_space);
    const int base_bar_height = 4;

    int bars = wifi_bar_count(signal);
    for (int i = 0; i < 5; i++)
    {
      int x = panel.x + bar_begin + (bar_width + 1) * i;
      int h = base_bar_height * (i + 1);
      int y = panel.y + panel.height - padding_y - h;

      if (i > bars)
        display.rectangle(x, y, bar_width, h);
      else
        display.filled_rectangle(x, y, bar_width, h);
    }
  }

  void Controller::show_hsv(esphome::display::Display &display)
  {
    RotarySettings hue_settings = rotary_settings[LIGHT_HUE];
    RotarySettings sat_settings = rotary_settings[LIGHT_SATURATION];
    display.printf(padding_x, padding_y,
                   resources.medium, "H: %3d", hue_settings.value);
    display.printf(padding_x, display_height / 4,
                   resources.medium, "S: %3d", sat_settings.value);

    int x = (3 * display_width) / 4;
    int y = display_half_height;

    int outer_radius = (display_width >> 2) - 1;
    display.circle(x, y, outer_radius);

    int radius = outer_radius - (padding_x << 1);
    display.circle(x, y, radius);

    int inner_radius = (radius * (sat_settings.max_value - sat_settings.value)) /
                       sat_settings.max_value;
    display.filled_circle(x, y, inner_radius);

    // 0 degrees x2 = x, y2 = y - r
    // 90 degrees x2 = x + r, y2 = y
    // 180 degs  x2 = x, y2 = y + r
    const float angle = static_cast<float>(rotary_settings[LIGHT_HUE].value);
    const float phi = angle * MY_PI / 180.0f;
    float dx = std::cos(phi) * outer_radius;
    float dy = std::sin(phi) * outer_radius;
    // display.printf(padding_x, display_height / 2,
    //                resources.medium, "dx: %3.0f", dx);
    // display.printf(padding_x, 3 * display_height / 4,
    //                resources.medium, "dy: %3.0f", dy);
    display.line(x, y, x + static_cast<int>(dx), y + static_cast<int>(dy));
  }

  void Controller::show_colors_hsv(esphome::display::Display &display)
  {
    show_color_guage(display, panels[LIGHT_HUE], rotary_settings[LIGHT_HUE], "H");
    show_color_guage(display, panels[LIGHT_SATURATION], rotary_settings[LIGHT_SATURATION], "S");
    show_color_guage(display, panels[LIGHT_VALUE], rotary_settings[LIGHT_VALUE], "V");
  }

  void Controller::show_colors(esphome::display::Display &display)
  {
    show_color_guage(display, panels[LIGHT_RED], rotary_settings[LIGHT_RED], "R");
    show_color_guage(display, panels[LIGHT_GREEN], rotary_settings[LIGHT_GREEN], "G");
    show_color_guage(display, panels[LIGHT_BLUE], rotary_settings[LIGHT_BLUE], "B");
  }

  void Controller::show_color_guage(esphome::display::Display &display,
                                    Panel &panel,
                                    RotarySettings &settings,
                                    const char *prefix)
  {
    int intensity = settings.value;
    display.print(panel.x + padding_x,
                  panel.y + padding_y,
                  resources.large, prefix);
    display.printf(panel.x + padding_x,
                   panel.y + panel.height / 2,
                   resources.medium, "%3d", intensity);

    const int bar_width = 8;
    const int x = panel.x + panel.width - padding_x - bar_width;
    const int y = panel.y + padding_y << 2;
    const int max_height = panel.height - (padding_y << 3);
    display.rectangle(panel.x + panel.width - padding_x - bar_width,
                      y, bar_width, max_height);

    const int bar_height = (intensity * max_height) / (settings.max_value - settings.min_value);
    display.filled_rectangle(x, y + max_height - bar_height, bar_width, bar_height);
  }

  void Controller::show_bulb_intensity(esphome::display::Display &display,
                                       Panel &panel,
                                       RotarySettings &settings)
  {
    int intensity = settings.value;
    display.printf(panel.x + padding_x,
                   panel.y + padding_y,
                   resources.medium, "%3d", intensity);
    auto image = light_bulb(intensity);
    display.image(panel.x + panel_width - image->get_width(), panel.y, image);
  }

  void Controller::show_brightness(esphome::display::Display &display)
  {
    show_bulb_intensity(display, panels[LIGHT_BRIGHTNESS], rotary_settings[LIGHT_BRIGHTNESS]);
  }

  void Controller::show_text(esphome::display::Display &display,
                             BaseFont *font,
                             std::string text, Panel &panel)
  {
    int width, x_offset, baseline, line_height;
    font->measure(text.c_str(), &width, &x_offset, &baseline, &line_height);
    int max_width = panel.width - padding_x * 2;
    int x = panel.x;

    if (!is_edit)
      x += padding_x;
    int y = panel.y + padding_y;
    if (is_edit)
      y += 10;
    int line = 0;
    const int max_line = panel_height / baseline;

    std::size_t found = 0;
    if (width > max_width)
    {
      found = text.find_first_of(" -_", found);
      if (found != std::string::npos)
      {
        display.print(x + max_width / 2, y, font,
                      esphome::display::TextAlign::CENTER_HORIZONTAL,
                      text.substr(0, found).c_str());
        text = text.substr(found + 1);
        y += baseline - 2;
        x += padding_x;
      }
    }
    else
    {
      y += baseline / 2;
    }

    display.print(x + max_width / 2, y, font,
                  esphome::display::TextAlign::CENTER_HORIZONTAL,
                  text.c_str());
  }

  void Controller::show_effect(esphome::display::Display &display)
  {
    show_text(display, resources.small, effect_name, panels[LIGHT_EFFECT]);
  }

  void Controller::show_image(esphome::display::Display &display,
                              esphome::display::BaseImage *image, PanelPos pos)
  {
    if (image != NULL)
    {
      const int height = image->get_height();
      const int width = image->get_width();
      const int half_width = image->get_width() >> 1;
      int x = 0;
      int y = 0;
      switch (pos)
      {
      case TOP_LEFT:
        break;
      case TOP_RIGHT:
        x = display_width - width;
        break;
      case TOP_MIDDLE:
        x = display_half_width - half_width;
        break;
      case BOTTOM_LEFT:
        y = display_height - height;
        break;
      case BOTTOM_RIGHT:
        x = display_width - width;
        y = display_height - height;
        break;
      case BOTTOM_MIDDLE:
        x = display_half_width - half_width;
        y = display_height - height;
        break;
      default:
        break;
      }
      display.image(x, y, image);
    }
  }

  void Controller::edit_gauge(esphome::display::Display &display,
                              RotarySettings &settings)
  {
    int value = settings.value;
    draw_guage(display, value, settings.max_value - settings.min_value);

    // const int guage_max = display_width - (padding_x << 1);
    // const int guage_height = display_half_height - (padding_y << 1);
    // display.rectangle(padding_x, padding_y, guage_max, guage_height);

    // const int guage_length = static_cast<int>((value * guage_max) /
    //                                           (settings.max_value - settings.min_value));
    // display.filled_rectangle(padding_x, padding_y, guage_length, guage_height);

    int y = display_half_height;
    int x = display_half_width;

    char buffer[8];
    snprintf(buffer, sizeof(buffer), "%3d", value);
    int width, x_offset, baseline, line_height;
    resources.large->measure(buffer, &width, &x_offset, &baseline, &line_height);
    display.print(x - width - padding_x, y, resources.large, buffer);
    auto image = light_bulb(value);
    display.image(x + padding_x, y, image);
  }

  void Controller::edit_effect(esphome::display::Display &display)
  {
    auto settings = rotary_settings[LIGHT_EFFECT];
    auto index = settings.value;
    if (index > settings.min_value)
      show_image(display, resources.up, TOP_MIDDLE);
    if (index < settings.max_value)
      show_image(display, resources.down, BOTTOM_MIDDLE);
    show_text(display, resources.medium, effect_name, edit_panel);
    show_index(display, settings);
  }

  void Controller::show_index(esphome::display::Display &display,
                              RotarySettings &settings)
  {
    char buffer[12];
    auto index = settings.value + 1;
    auto count = settings.max_value + 1;
    snprintf(buffer, sizeof(buffer), "%d/%d", index, count);

    int width, x_offset, baseline, line_height;
    resources.small->measure(buffer, &width, &x_offset, &baseline, &line_height);

    display.printf(display_width - width, display_height - baseline,
                   resources.small, buffer);
  }

  void Controller::edit_wifi_signal(esphome::display::Display &display)
  {
    auto settings = rotary_settings[WIFI_SIGNAL];
    int index = settings.value;
    if (index < settings.min_value || index > settings.max_value)
    {
      show_index(display, settings);
      return;
    }

    esphome::text_sensor::TextSensor *info = wifi_info[index];
    if (info == NULL)
    {
      display.print(display_half_width, display_half_height,
                    resources.medium,
                    esphome::display::TextAlign::CENTER,
                    "unavailable");
      show_index(display, settings);
      return;
    }

    display.print(display_half_width, padding_y,
                  resources.medium,
                  esphome::display::TextAlign::CENTER_HORIZONTAL,
                  info->get_name().c_str());

    int width, x_offset, baseline, line_height;
    const char *text = info->get_state().c_str();
    auto font = resources.large;
    font->measure(text, &width, &x_offset, &baseline, &line_height);
    int y = padding_y + baseline;

    if (width > display_width)
    {
      font = resources.medium;
      font->measure(text, &width, &x_offset, &baseline, &line_height);
      if (width > display_width)
      {
        font = resources.small;
        font->measure(text, &width, &x_offset, &baseline, &line_height);
      }
    }

    if (width > display_width)
    {
      const std::string &state = info->get_state();
      auto length = state.size();
      display.print(display_half_width, y,
                    font,
                    esphome::display::TextAlign::CENTER_HORIZONTAL,
                    state.substr(0, length / 2).c_str());
      display.print(display_half_width, y + baseline,
                    font,
                    esphome::display::TextAlign::CENTER_HORIZONTAL,
                    state.substr(length / 2).c_str());
    }
    else
    {
      display.print(display_half_width, y,
                    font,
                    esphome::display::TextAlign::CENTER_HORIZONTAL,
                    info->get_state().c_str());
    }

    show_index(display, settings);
  }
}