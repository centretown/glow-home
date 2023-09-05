#pragma once

#include "esphome.h"
#include "panel.h"

using esphome::display::BaseFont;
using esphome::display::BaseImage;
using esphome::rotary_encoder::ROTARY_ENCODER_1_PULSE_PER_CYCLE;
using esphome::rotary_encoder::ROTARY_ENCODER_2_PULSES_PER_CYCLE;
using esphome::rotary_encoder::ROTARY_ENCODER_4_PULSES_PER_CYCLE;
using esphome::rotary_encoder::RotaryEncoderResolution;

namespace rgb_panels
{
    enum PanelID : uint32_t
    {
        LIGHT_EFFECT,
        LIGHT_BRIGHTNESS,
        WIFI_SIGNAL,
        LIGHT_RED,
        LIGHT_GREEN,
        LIGHT_BLUE,
        LIGHT_HUE,
        LIGHT_SATURATION,
        LIGHT_VALUE,
        PANEL_COUNT,
    };

    enum InfoID : uint32_t
    {
        IP_ADDRESS,
        SSID,
        BSSID,
        MAC_ADDRESS,
        INFO_COUNT,
    };

    struct Resources
    {
        BaseFont *small;
        BaseFont *medium;
        BaseFont *large;
        BaseImage *up;
        BaseImage *down;
        BaseImage *effect;
        BaseImage *bulbs[11];
    };

    class Controller
    {
    private:
        PanelID panel_id = (PanelID)0;
        bool is_edit = false;
        std::string effect_name = "None";
        int wifi_signal = 0;

        static esphome::text_sensor::TextSensor *wifi_info[INFO_COUNT];

    public:
        static void setup(Resources *res);

        void on_click(esphome::light::AddressableLightState *light,
                      esphome::rotary_encoder::RotaryEncoderSensor *encoder);
        void on_rotate(esphome::light::AddressableLightState *light,
                       esphome::rotary_encoder::RotaryEncoderSensor *encoder);

        void on_light_state(esphome::light::AddressableLightState *state);

        void on_wifi_signal(float signal)
        {
            wifi_signal = static_cast<int>(signal);
        }

        void on_info(esphome::text_sensor::TextSensor *info,
                     InfoID id)
        {
            if (info->has_state() && id < INFO_COUNT)
                wifi_info[id] = info;
        }

        void show(esphome::display::Display &display);

        // const bool is_edit() ALWAYS_INLINE { return edit; }

    private:
        Panel panels[PANEL_COUNT] = {
            {0, 0, display_width, panel_height},                    //
            {0, panel_height, panel_width, panel_height},           //
            {panel_width, panel_height, panel_width, panel_height}, //
            // rgb
            {0, 0, display_third_width, display_height},                        //
            {display_third_width, 0, display_third_width, display_height},      //
            {display_third_width << 1, 0, display_third_width, display_height}, //
            // hsv
            {0, 0, display_third_width, display_height},                        //
            {display_third_width, 0, display_third_width, display_height},      //
            {display_third_width << 1, 0, display_third_width, display_height}, //
        };

        RotarySettings rotary_settings[PANEL_COUNT]{
            {ROTARY_ENCODER_1_PULSE_PER_CYCLE, -1, 100, -1},
            {ROTARY_ENCODER_4_PULSES_PER_CYCLE, 0, 100, 0},
            {ROTARY_ENCODER_1_PULSE_PER_CYCLE, 0, INFO_COUNT - 1, 0},
            // rgb
            {ROTARY_ENCODER_4_PULSES_PER_CYCLE, 0, 100, 0},
            {ROTARY_ENCODER_4_PULSES_PER_CYCLE, 0, 100, 0},
            {ROTARY_ENCODER_4_PULSES_PER_CYCLE, 0, 100, 0},
            // hsv
            {ROTARY_ENCODER_4_PULSES_PER_CYCLE, 0, 360, 0},
            {ROTARY_ENCODER_4_PULSES_PER_CYCLE, 0, 100, 0},
            {ROTARY_ENCODER_4_PULSES_PER_CYCLE, 0, 100, 0},
        };

        void edit(esphome::display::Display &display);

        int wifi_bar_count(float signal);

        Panel &check_panel(PanelID &panel_id)
        {
            panel_id = verify_panel_id(panel_id);
            return panels[panel_id];
        }

        void toggle_edit() ALWAYS_INLINE { is_edit = !is_edit; }

        const PanelID verify_panel_id(PanelID id) ALWAYS_INLINE
        {
            return (id >= PANEL_COUNT) ? (PanelID)0 : id;
        }

        int find_effect_index(esphome::light::AddressableLightState *light_state);

        void encode_rotary(esphome::rotary_encoder::RotaryEncoderSensor *encoder,
                           RotarySettings &settings);
        void encode_effect(esphome::light::AddressableLightState *light_state,
                           esphome::rotary_encoder::RotaryEncoderSensor *encoder);
        void encode_select(esphome::rotary_encoder::RotaryEncoderSensor *encoder);

        void show_all(esphome::display::Display &display);

        void show_wifi_signal(esphome::display::Display &display);
        void show_brightness(esphome::display::Display &display);
        void show_effect(esphome::display::Display &display);

        void edit_gauge(esphome::display::Display &display,
                        RotarySettings &settings);
        void edit_effect(esphome::display::Display &display);
        void edit_wifi_signal(esphome::display::Display &display);

        void show_text(esphome::display::Display &display,
                       BaseFont *font,
                       std::string text, Panel &panel);
        void show_image(esphome::display::Display &display,
                        esphome::display::BaseImage *image, PanelPos pos);
        void show_border(esphome::display::Display &display,
                         const Panel &panel) ALWAYS_INLINE
        {
            display.rectangle(panel.x, panel.y,
                              panel.width, panel.height);
        }
        void show_index(esphome::display::Display &display,
                        RotarySettings &settings);
        void show_bulb_intensity(esphome::display::Display &display,
                                 Panel &panel,
                                 RotarySettings &settings);
        void show_color_guage(esphome::display::Display &display,
                              Panel &panel,
                              RotarySettings &settings, const char *prefix = "");
        void show_colors(esphome::display::Display &display);
        void show_colors_hsv(esphome::display::Display &display);
        void on_rotate_hsv(esphome::light::LightCall &call, float sensor_value);
        void show_hsv(esphome::display::Display &display);
    };

    Controller &control();

}