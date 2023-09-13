
#include "control.h"
#include <cmath>
#include <string.h>
#include <algorithm>
#include <charconv>

namespace panels
{
  constexpr int padding_x = 3;
  constexpr int padding_y = 2;
  constexpr float MY_PI = 3.1415926535897932384626433832795;

  esphome::display::BaseImage *Controller::light_bulb(int brightness)
  {
    return resources.bulbs[(brightness / 10) % 11];
  }

  void draw_guage(esphome::display::Display &display, int value, int range)
  {
    const int guage_max = display_width - (padding_x << 1);
    const int guage_height = display_half_height - (padding_y << 1);
    display.rectangle(padding_x, padding_y, guage_max, guage_height);
    const int guage_length = (value * guage_max) / range;
    display.filled_rectangle(padding_x, padding_y, guage_length, guage_height);
  }

  void Controller::draw_bulbs(esphome::display::Display &display,
                              int x, int y, int value)
  {
    char buffer[8];
    snprintf(buffer, sizeof(buffer), "%3d", value);
    int width, x_offset, baseline, line_height;
    resources.large->measure(buffer, &width, &x_offset, &baseline, &line_height);
    display.print(x - width - padding_x, y, resources.large, buffer);
    display.image(x + padding_x, y, light_bulb(value));
  }

  const std::vector<float> wifi_signals =
      {-44.0, -67.0, -70.0, -80.0};

  int Controller::wifi_bar_count(float signal)
  {
    if (std::isnan(signal))
    {
      return 0;
    }

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
    uint32_t now = esphome::millis();
    if (now > next)
    {
      const int image_count = sizeof(resources.idle) / sizeof(resources.idle[0]);
      sleeping = true;
      now = (now - next) / MAX_TIME;
      display.image(display_half_width, 0,
                    resources.idle[now % image_count],
                    esphome::display::ImageAlign::TOP_CENTER);
      return;
    }

    if (is_edit)
      edit(display);
    else
      show_all(display);
  }

  void Controller::show_all(esphome::display::Display &display)
  {
    if (panel_id < RED_VALUE)
    {
      show_brightness(display);
      show_effect(display);
      show_wifi_signal(display);
    }
    else if (panel_id < HUE)
    {
      show_colors(display);
    }
    else if (panel_id < PANEL_COUNT)
    {
      show_hsv(display);
    }
    else
    {
      return;
    }
    show_border(display, panels[panel_id]);
  }

  void Controller::edit(esphome::display::Display &display)
  {
    switch (panel_id)
    {
    case EFFECT:
      edit_effect(display);
      break;
    case WIFI_SIGNAL:
      edit_wifi_signal(display);
      break;
    case BRIGHTNESS:
      edit_gauge(display, rotary_states[BRIGHTNESS]);
      break;
    default:
      if (panel_id < HUE)
      {
        show_colors(display);
      }
      else if (panel_id < PANEL_COUNT)
      {
        show_hsv(display);
      }
      break;
    }
  }

  void Controller::show_wifi_signal(esphome::display::Display &display)
  {
    Panel &panel = panels[WIFI_SIGNAL];

    int signal = wifi_signal;
    signal = std::min(std::max(2 * (signal + 100), 0), 100);
    display.printf(panel.x + padding_x, panel.y + padding_y,
                   resources.medium, "%d", signal);

    constexpr int bar_count = 5;
    constexpr int bar_width = 6;
    constexpr int bar_space = 1;
    constexpr int base_bar_height = 4;
    const int bar_begin = panel_width - padding_x - bar_count * (bar_width + bar_space);

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
    char buffer[80];

    auto centre = [&](PanelID id)
    {
      RotaryState &state = rotary_states[id];
      Panel &panel = panels[id];
      display.print(panel.x + (panel.width >> 1), panel.y + (panel.height >> 1),
                    (is_edit && panel_id == id) ? resources.large : resources.medium,
                    esphome::display::TextAlign::CENTER, buffer);
    };

    snprintf(buffer, sizeof(buffer), "H: %3d", rotary_states[HUE].value);
    centre(HUE);

    snprintf(buffer, sizeof(buffer), "S: %3d", rotary_states[SATURATION].value);
    centre(SATURATION);

    show_wheel(display);
  }

  void Controller::show_wheel(esphome::display::Display &display)
  {
    const int x = ((3 * display_width) >> 2);
    const int y = display_half_height;

    const int radius = (display_width >> 2) - 5;
    display.circle(x, y, radius);

    const float angle = static_cast<float>(rotary_states[HUE].value);
    const float phi = angle * MY_PI / 180.0f;

    const float dx = std::cos(phi) * radius;
    const float dy = std::sin(phi) * radius;
    display.line(x, y, x + static_cast<int>(dx), y + static_cast<int>(dy));

    const float saturation = static_cast<float>(rotary_states[SATURATION].value);
    const float sx = dx * saturation / 100.0f;
    const float sy = dy * saturation / 100.0f;

    display.circle(x + static_cast<int>(sx),
                   y + static_cast<int>(sy), 5);
  }

  void Controller::show_colors(esphome::display::Display &display)
  {
    show_color_guage(display, RED_VALUE, "R:");
    show_color_guage(display, GREEN_VALUE, "G:");
    show_color_guage(display, BLUE_VALUE, "B:");
  }

  void Controller::show_color_guage(esphome::display::Display &display,
                                    PanelID id, const char *prefix)
  {
    Panel &panel = panels[id];
    RotaryState &rotary_state = rotary_states[id];

    int intensity = rotary_state.value;
    display.print(panel.x + padding_x,
                  panel.y + padding_y,
                  (is_edit && id == panel_id)
                      ? resources.large
                      : resources.medium,
                  prefix);
    display.printf(panel.x + padding_x,
                   panel.y + panel.height / 2,
                   resources.medium, "%3d", intensity);

    const int bar_width = 8;
    const int x = panel.x + panel.width - padding_x - bar_width;
    const int y = panel.y + padding_y << 2;
    const int max_height = panel.height - (padding_y << 3);
    display.rectangle(panel.x + panel.width - padding_x - bar_width,
                      y, bar_width, max_height);

    const int bar_height = (intensity * max_height) / (rotary_state.max_value - rotary_state.min_value);
    display.filled_rectangle(x, y + max_height - bar_height, bar_width, bar_height);
  }

  void Controller::show_bulb_intensity(esphome::display::Display &display,
                                       Panel &panel,
                                       RotaryState &rotary_state)
  {
    int intensity = rotary_state.value;
    display.printf(panel.x + padding_x,
                   panel.y + padding_y,
                   resources.medium, "%3d", intensity);
    auto image = light_bulb(intensity);
    display.image(panel.x + panel_width - image->get_width(), panel.y, image);
  }

  std::size_t Controller::split(std::string text, std::string sections[], std::size_t length)
  {
    std::size_t start = 0;
    std::size_t found = 0;
    std::size_t count = 0;

    for (; count < length - 1; ++count)
    {
      found = text.find_first_of(" -_", found);
      if (found == std::string::npos)
      {
        break;
      }

      sections[count] = text.substr(start, found - start);
      start = ++found;
    }

    sections[count] = text.substr(start);
    return count + 1;
  }

  void Controller::split_effect(esphome::display::Display &display, Panel &panel,
                                std::string text)
  {
    auto font = (is_edit) ? resources.medium : resources.small;
    int width, x_offset, baseline, line_height;
    font->measure(text.c_str(), &width, &x_offset, &baseline, &line_height);

    int x = panel.width >> 1;
    int y = (is_edit) ? 14 : padding_y;
    int y2 = y + baseline - 2;

    std::size_t start = 0;

    constexpr std::size_t MAX_SECTIONS = 3;
    std::string sections[MAX_SECTIONS];
    std::size_t count = split(text, sections, MAX_SECTIONS);

    switch (count)
    {
    case 1:
      display.print(x, y, font,
                    esphome::display::TextAlign::CENTER_HORIZONTAL,
                    sections[0].c_str());
      break;
    case 2:
      display.print(x, y, font,
                    esphome::display::TextAlign::CENTER_HORIZONTAL,
                    sections[0].c_str());
      display.print(x, y2, font,
                    esphome::display::TextAlign::CENTER_HORIZONTAL,
                    sections[1].c_str());
      break;
    case 3:
      display.print(x, y, font,
                    esphome::display::TextAlign::CENTER_HORIZONTAL,
                    sections[0].c_str());
      display.print(x, y2, font,
                    esphome::display::TextAlign::CENTER_HORIZONTAL,
                    (sections[1] + " " + sections[2]).c_str());
      break;
    }
  }

  void Controller::show_effect(esphome::display::Display &display)
  {
    Panel &panel = panels[EFFECT];
    split_effect(display, panel, effect_name);
    display.image(panel.x, panel.y, resources.effect);
    display.image(panel.x + panel.width - resources.effect->get_width(),
                  panel.y, resources.effect);
  }

  void Controller::edit_gauge(esphome::display::Display &display,
                              RotaryState &rotary_state)
  {
    int value = rotary_state.value;
    draw_guage(display, value, rotary_state.max_value - rotary_state.min_value);

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
    Panel full_panel =
        {0, 0, display_width, display_height};
    split_effect(display, full_panel, effect_name);
    show_index(display, rotary_states[EFFECT]);
    display.image(0, 0, resources.effect);
    display.image(display_width - resources.effect->get_width(),
                  0, resources.effect);
  }

  void Controller::show_index(esphome::display::Display &display,
                              RotaryState &rotary_state)
  {
    char buffer[12];
    snprintf(buffer, sizeof(buffer), "%d/%d",
             rotary_state.value + 1,
             rotary_state.max_value + 1);
    display.print(display_width - 1, display_height - 1, resources.small,
                  esphome::display::TextAlign::BOTTOM_RIGHT,
                  buffer);
  }

  void Controller::write_panel(esphome::display::Display &display,
                               const char *title, esphome::display::BaseFont *title_font,
                               const char *value, esphome::display::BaseFont *value_font)
  {
    display.print(display_half_width, 0,
                  title_font,
                  esphome::display::TextAlign::CENTER_HORIZONTAL,
                  title);
    display.print(display_half_width, display_half_height,
                  value_font,
                  esphome::display::TextAlign::CENTER,
                  value);
    show_index(display, rotary_states[WIFI_SIGNAL]);
  };

  void Controller::edit_wifi_signal(esphome::display::Display &display)
  {
    int index = rotary_states[WIFI_SIGNAL].value;

    if (index == WIFI_STRENGTH)
    {
      char buffer[12];
      snprintf(buffer, sizeof(buffer), "%3d dBm", wifi_signal);
      write_panel(display, "WiFi Signal", resources.large,
                  buffer, resources.medium);
      return;
    }

    esphome::text_sensor::TextSensor *info = wifi_info[index];
    if (info == NULL)
    {
      write_panel(display, "Missing", resources.large,
                  "unavailable", resources.medium);
      return;
    }

    auto value = info->get_state().c_str();
    auto value_font = resources.medium;
    int width, x_offset, baseline, line_height;
    value_font->measure(value, &width, &x_offset, &baseline, &line_height);
    if (width > display_width)
    {
      value_font = resources.small;
    }

    write_panel(display,
                info->get_name().c_str(), resources.large,
                value, value_font);
  }
}