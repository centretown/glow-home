#pragma once

#include "esphome.h"
#include "panel.h"
#include "rotary.h"

using esphome::display::BaseFont;
using esphome::display::BaseImage;
using esphome::rotary_encoder::ROTARY_ENCODER_1_PULSE_PER_CYCLE;
using esphome::rotary_encoder::ROTARY_ENCODER_2_PULSES_PER_CYCLE;
using esphome::rotary_encoder::ROTARY_ENCODER_4_PULSES_PER_CYCLE;
using esphome::rotary_encoder::RotaryEncoderResolution;

namespace panels
{
    constexpr uint32_t MAX_TIME = 1000 * 10;

    enum PanelID : int32_t
    {
        EFFECT,
        BRIGHTNESS,
        WIFI_SIGNAL,
        RED_VALUE,
        GREEN_VALUE,
        BLUE_VALUE,
        HUE,
        SATURATION,
        PANEL_COUNT,
    };

    enum InfoID : uint32_t
    {
        IP_ADDRESS,
        SSID,
        BSSID,
        MAC_ADDRESS,
        SENSOR_INFO_COUNT,
        WIFI_STRENGTH = SENSOR_INFO_COUNT,
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
        BaseImage *idle[11];
    };

    class Controller
    {
    private:
        PanelID panel_id = PanelID(0);
        bool is_edit = false;
        std::string effect_name = "None";
        int wifi_signal = 0;
        static esphome::text_sensor::TextSensor *wifi_info[SENSOR_INFO_COUNT];

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
            {
                NULL,
            },
        };

        Panel panels[PANEL_COUNT] = {
            // effect, brightness, wifi
            {0, 0, display_width, panel_height},                    //
            {0, panel_height, panel_width, panel_height},           //
            {panel_width, panel_height, panel_width, panel_height}, //
            // rgb
            {0, 0, display_third_width, display_height},                        //
            {display_third_width, 0, display_third_width, display_height},      //
            {display_third_width << 1, 0, display_third_width, display_height}, //
            // hsv
            {0, 0, panel_width, panel_height},            //
            {0, panel_height, panel_width, panel_height}, //
        };

        RotaryState selector =
            {ROTARY_ENCODER_1_PULSE_PER_CYCLE, 0, PANEL_COUNT - 1, 0, true};

        RotaryState rotary_states[PANEL_COUNT]{
            // effect, brightness, wifi
            {ROTARY_ENCODER_1_PULSE_PER_CYCLE, -1, 100, -1, true},
            {ROTARY_ENCODER_4_PULSES_PER_CYCLE, 0, 100, 0, false},
            {ROTARY_ENCODER_1_PULSE_PER_CYCLE, 0, INFO_COUNT - 1, 0, true},
            // rgb
            {ROTARY_ENCODER_4_PULSES_PER_CYCLE, 0, 100, 0, false},
            {ROTARY_ENCODER_4_PULSES_PER_CYCLE, 0, 100, 0, false},
            {ROTARY_ENCODER_4_PULSES_PER_CYCLE, 0, 100, 0, false},
            // hsv
            {ROTARY_ENCODER_4_PULSES_PER_CYCLE, 0, 360, 0, true},
            {ROTARY_ENCODER_4_PULSES_PER_CYCLE, 0, 100, 0, false},
        };

        uint32_t next = millis() + MAX_TIME;
        bool sleeping = false;

    public:
        void setup(Resources *res, esphome::rotary_encoder::RotaryEncoderSensor *encoder);

        void on_click(esphome::light::AddressableLightState *light,
                      esphome::rotary_encoder::RotaryEncoderSensor *encoder);
        void on_rotate(esphome::light::AddressableLightState *light,
                       esphome::rotary_encoder::RotaryEncoderSensor *encoder);

        void on_light_state(esphome::light::AddressableLightState *state);

        void on_wifi_signal(float signal) ALWAYS_INLINE
        {
            wifi_signal = static_cast<int>(signal);
        }

        void on_info_ip_address(esphome::text_sensor::TextSensor *info) ALWAYS_INLINE
        {
            wifi_info[IP_ADDRESS] = info;
        }
        void on_info_ssid(esphome::text_sensor::TextSensor *info) ALWAYS_INLINE
        {
            info->get_state();
            wifi_info[SSID] = info;
        }
        void on_info_bssid(esphome::text_sensor::TextSensor *info) ALWAYS_INLINE
        {
            wifi_info[BSSID] = info;
        }
        void on_info_mac_address(esphome::text_sensor::TextSensor *info) ALWAYS_INLINE
        {
            wifi_info[MAC_ADDRESS] = info;
        }

        void show(esphome::display::Display &display);

    private:
        bool wake_up()
        {
            next = millis() + MAX_TIME;
            if (sleeping)
            {
                sleeping = false;
                return true;
            }
            return false;
        }

        void draw_bulbs(esphome::display::Display &display,
                        int x, int y, int value);

        esphome::display::BaseImage *light_bulb(int brightness);

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

        int validate_rotary_state(esphome::rotary_encoder::RotaryEncoderSensor *encoder,
                                  RotaryState &settings);

        int find_effect_index(esphome::light::AddressableLightState *light_state);

        void on_rotate_hsv(esphome::light::LightCall &call, float sensor_value);

        void encode_rotary(esphome::rotary_encoder::RotaryEncoderSensor *encoder,
                           RotaryState &settings);

        void show_all(esphome::display::Display &display);

        void show_wifi_signal(esphome::display::Display &display);
        void show_effect(esphome::display::Display &display);

        void edit_gauge(esphome::display::Display &display,
                        RotaryState &settings);
        void edit_effect(esphome::display::Display &display);
        void edit_wifi_signal(esphome::display::Display &display);

        std::size_t split(std::string text, std::string sections[], std::size_t length);
        // std::size_t split(std::string text, std::size_t sections[], std::size_t length);

        void split_effect(esphome::display::Display &display,
                          Panel &panel, std::string LOG_TEXT_SENSOR);

        void show_border(esphome::display::Display &display,
                         const Panel &panel) ALWAYS_INLINE
        {
            display.rectangle(panel.x, panel.y,
                              panel.width, panel.height);
        }

        void show_brightness(esphome::display::Display &display) ALWAYS_INLINE
        {
            show_bulb_intensity(display, panels[BRIGHTNESS],
                                rotary_states[BRIGHTNESS]);
        }

        void show_index(esphome::display::Display &display,
                        RotaryState &settings);
        void show_bulb_intensity(esphome::display::Display &display,
                                 Panel &panel,
                                 RotaryState &settings);
        void show_color_guage(esphome::display::Display &display,
                              PanelID id, const char *prefix = "");
        void show_colors(esphome::display::Display &display);

        void show_hsv(esphome::display::Display &display);
        void show_wheel(esphome::display::Display &display);
        void write_panel(esphome::display::Display &display,
                         const char *title, esphome::display::BaseFont *title_font,
                         const char *value, esphome::display::BaseFont *value_font);
    };

    Controller &control();

}