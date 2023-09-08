#include "control.h"
#include "../glow/HSVColor.h"
#include "../glow/RGBColor.h"

namespace panels
{
    Controller controller;
    Controller &control() { return controller; };

    esphome::text_sensor::TextSensor *Controller::wifi_info[INFO_COUNT] = {
        NULL,
    };

    void Controller::setup(Resources *res,
                           esphome::rotary_encoder::RotaryEncoderSensor *encoder)
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

        encode_select(encoder);
    };

    void Controller::on_light_state(esphome::light::AddressableLightState *state)
    {
        float brightness = state->remote_values.get_brightness();
        float red = state->remote_values.get_red();
        float green = state->remote_values.get_green();
        float blue = state->remote_values.get_blue();

        rotary_states[LIGHT_EFFECT].value = find_effect_index(state);
        rotary_states[LIGHT_BRIGHTNESS].value = static_cast<int>(brightness * 100.0f);
        rotary_states[LIGHT_RED].value = static_cast<int>(red * 100.0f);
        rotary_states[LIGHT_GREEN].value = static_cast<int>(green * 100.0f);
        rotary_states[LIGHT_BLUE].value = static_cast<int>(blue * 100.0f);

        glow::HSVColor hsv;
        hsv.from_rgb(glow::Color(static_cast<uint8_t>(red * 255.0f),
                                 static_cast<uint8_t>(green * 255.0f),
                                 static_cast<uint8_t>(blue * 255.0f)));

        rotary_states[LIGHT_HUE].value = (hsv.hue * 360) / glow::hue_limit;
        rotary_states[LIGHT_SATURATION].value = (hsv.saturation * 100) / glow::byte_limit;
        // rotary_settings[LIGHT_VALUE].value = (hsv.value * 100) / glow::byte_limit;
    }

    void Controller::on_rotate_hsv(esphome::light::LightCall &call, float sensor_value)
    {
        float f_value = 255.0f;
        float f_hue = 0.0;
        float f_saturation = 0.0;

        if (LIGHT_HUE == panel_id)
        {
            f_hue = sensor_value;
            f_saturation = static_cast<float>(rotary_states[LIGHT_SATURATION].value);
        }
        else if (LIGHT_SATURATION == panel_id)
        {
            f_hue = static_cast<float>(rotary_states[LIGHT_HUE].value);
            f_saturation = sensor_value;
        }

        glow::HSVColor hsv;
        hsv.from_color_wheel(f_hue, f_saturation, f_value);
        glow::Color color = hsv.to_rgb();

        call.set_rgb(static_cast<float>(color.red) / 255.0,
                     static_cast<float>(color.green) / 255.0,
                     static_cast<float>(color.blue) / 255.0);
    }

    int Controller::validate_rotary_state(esphome::rotary_encoder::RotaryEncoderSensor *encoder, RotaryState &rotary_state)
    {
        float encoder_state = encoder->get_state();
        int index = static_cast<int>(encoder_state);

        // wrap over/under
        if (index > rotary_state.max_value)
        {
            index = rotary_state.min_value;
            encoder->set_value(index);
        }
        else if (index < rotary_state.min_value)
        {
            index = rotary_state.max_value;
            encoder->set_value(index);
        }

        rotary_state.value = index;

        return index;
    }

    void Controller::on_rotate(esphome::light::AddressableLightState *state,
                               esphome::rotary_encoder::RotaryEncoderSensor *encoder)
    {
        if (!is_edit)
        {
            panel_id = (PanelID)validate_rotary_state(encoder, selector);
            return;
        }

        int32_t index = validate_rotary_state(encoder, rotary_states[panel_id]);

        if (WIFI_SIGNAL == panel_id)
        {
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

        float fractional_value = static_cast<float>(index) / 100.0f;
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
            on_rotate_hsv(call, encoder->get_state());
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

        panel_id = (PanelID)selector.value;

        if (panel_id == LIGHT_EFFECT)
        {
            encode_effect(state, encoder);
        }
        else if (panel_id < PANEL_COUNT)
        {
            encode_rotary(encoder, rotary_states[panel_id]);
        }
    }

    void Controller::encode_select(esphome::rotary_encoder::RotaryEncoderSensor *encoder)
    {
        encode_rotary(encoder, selector);
    }

    void Controller::encode_effect(esphome::light::AddressableLightState *state,
                                   esphome::rotary_encoder::RotaryEncoderSensor *encoder)
    {
        rotary_states[LIGHT_EFFECT].max_value = state->get_effects().size() - 1;
        encode_rotary(encoder, rotary_states[LIGHT_EFFECT]);
    }

    void Controller::encode_rotary(esphome::rotary_encoder::RotaryEncoderSensor *encoder,
                                   RotaryState &settings)
    {
        encoder->set_resolution(settings.mode);
        if (settings.wrap)
        {
            encoder->set_min_value(settings.min_value - 1);
            encoder->set_max_value(settings.max_value + 1);
        }
        else
        {
            encoder->set_min_value(settings.min_value);
            encoder->set_max_value(settings.max_value);
        }
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