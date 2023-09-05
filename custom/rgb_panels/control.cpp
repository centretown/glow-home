#include "control.h"
#include "../glow/HSVColor.h"
#include "../glow/RGBColor.h"

namespace rgb_panels
{
    Controller controller;
    Controller &control() { return controller; };

    void Controller::on_light_state(esphome::light::AddressableLightState *state)
    {
        float brightness = state->remote_values.get_brightness();
        float red = state->remote_values.get_red();
        float green = state->remote_values.get_green();
        float blue = state->remote_values.get_blue();

        rotary_settings[LIGHT_EFFECT].value = find_effect_index(state);
        rotary_settings[LIGHT_BRIGHTNESS].value = static_cast<int>(brightness * 100.0f);
        rotary_settings[LIGHT_RED].value = static_cast<int>(red * 100.0f);
        rotary_settings[LIGHT_GREEN].value = static_cast<int>(green * 100.0f);
        rotary_settings[LIGHT_BLUE].value = static_cast<int>(blue * 100.0f);

        glow::HSVColor hsv;
        hsv.from_rgb(glow::Color(static_cast<uint8_t>(red * 255.0f),
                                 static_cast<uint8_t>(green * 255.0f),
                                 static_cast<uint8_t>(blue * 255.0f)));

        rotary_settings[LIGHT_HUE].value = (hsv.hue * 360) / glow::hue_limit;
        rotary_settings[LIGHT_SATURATION].value = (hsv.saturation * 100) / glow::byte_limit;
        rotary_settings[LIGHT_VALUE].value = (hsv.value * 100) / glow::byte_limit;
    }

    void Controller::on_rotate_hsv(esphome::light::LightCall &call, float sensor_value)
    {
        float f_hue = static_cast<float>(rotary_settings[LIGHT_HUE].value);
        float f_saturation = static_cast<float>(rotary_settings[LIGHT_SATURATION].value);
        float f_value = static_cast<float>(rotary_settings[LIGHT_VALUE].value);
        if (LIGHT_HUE == panel_id)
        {
            f_hue = sensor_value;
        }
        else if (LIGHT_SATURATION == panel_id)
        {
            f_saturation = sensor_value;
        }
        else
        {
            f_value = sensor_value;
        }
        glow::HSVColor hsv;
        hsv.from_color_wheel(f_hue, f_saturation, f_value);
        glow::Color color = hsv.to_rgb();

        call.set_rgb(static_cast<float>(color.red) / 255.0,
                     static_cast<float>(color.green) / 255.0,
                     static_cast<float>(color.blue) / 255.0);
    }

    void Controller::on_rotate(esphome::light::AddressableLightState *state,
                               esphome::rotary_encoder::RotaryEncoderSensor *encoder)
    {
        if (!is_edit)
        {
            panel_id = static_cast<PanelID>(encoder->get_state());
            panel_id = verify_panel_id(panel_id);
            return;
        }

        float encoder_value = encoder->get_state();

        int32_t index = static_cast<int32_t>(encoder_value);
        if (WIFI_SIGNAL == panel_id)
        {
            rotary_settings[WIFI_SIGNAL].value = index;
            return;
        }

        esphome::light::LightCall call = state->make_call();
        auto effects = state->get_effects();

        if (LIGHT_EFFECT == panel_id)
        {
            if (index < 0)
            {
                call.set_effect("None");
            }
            else if (index < effects.size())
            {
                call.set_effect(effects[index]->get_name());
            }
            call.perform();
            return;
        }

        float fractional_value = encoder_value / 100.0f;
        switch (panel_id)
        {
        case LIGHT_BRIGHTNESS:
            call.set_brightness(fractional_value);
            break;
        case LIGHT_RED:
            call.set_red(fractional_value);
            break;
        case LIGHT_GREEN:
            call.set_green(fractional_value);
            break;
        case LIGHT_BLUE:
            call.set_blue(fractional_value);
            break;

        case LIGHT_HUE:
        case LIGHT_SATURATION:
        case LIGHT_VALUE:
            on_rotate_hsv(call, encoder_value);
            break;

        default:
            return;
        }
        call.perform();
    }

    void Controller::on_click(esphome::light::AddressableLightState *state,
                              esphome::rotary_encoder::RotaryEncoderSensor *encoder)
    {
        toggle_edit();
        if (!is_edit)
        {
            encode_select(encoder);
            return;
        }

        if (panel_id == LIGHT_EFFECT)
        {
            encode_effect(state, encoder);
        }
        else if (panel_id < PANEL_COUNT)
        {
            encode_rotary(encoder, rotary_settings[panel_id]);
        }
    }

    void Controller::encode_select(esphome::rotary_encoder::RotaryEncoderSensor *encoder)
    {
        encoder->set_resolution(ROTARY_ENCODER_1_PULSE_PER_CYCLE);
        encoder->set_min_value(0);
        encoder->set_max_value(PANEL_COUNT - 1);
        encoder->set_value(panel_id);
    }

    void Controller::encode_rotary(esphome::rotary_encoder::RotaryEncoderSensor *encoder,
                                   RotarySettings &settings)
    {
        encoder->set_resolution(settings.mode);
        encoder->set_min_value(settings.min_value);
        encoder->set_max_value(settings.max_value);
        encoder->set_value(settings.value);
    }

    void Controller::encode_effect(esphome::light::AddressableLightState *state,
                                   esphome::rotary_encoder::RotaryEncoderSensor *encoder)
    {
        RotarySettings &settings = rotary_settings[LIGHT_EFFECT];
        auto effects = state->get_effects();

        encoder->set_resolution(settings.mode);
        settings.min_value = -1;
        encoder->set_min_value(settings.min_value);
        settings.max_value = effects.size() - 1;
        encoder->set_max_value(settings.max_value);
        settings.value = find_effect_index(state);
        encoder->set_value(settings.value);
    }

    int Controller::find_effect_index(esphome::light::AddressableLightState *state)
    {
        effect_name = state->get_effect_name();
        auto effects = state->get_effects();
        std::size_t index = 0;
        for (auto effect : effects)
        {
            if (effect->get_name() == effect_name)
            {
                return index;
            }
            index++;
        }
        return -1;
    }
}