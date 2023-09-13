#include "control.h"
#include "../glow/HSVColor.h"
#include "../glow/RGBColor.h"

namespace panels
{
    Controller controller;
    Controller &control() { return controller; };

    esphome::text_sensor::TextSensor *Controller::wifi_info[SENSOR_INFO_COUNT] = {};

    void Controller::setup(Resources *res,
                           esphome::rotary_encoder::RotaryEncoderSensor *encoder)
    {
        resources.small = res->small;
        resources.medium = res->medium;
        resources.large = res->large;
        resources.up = res->up;
        resources.down = res->down;
        resources.effect = res->effect;
        auto count = sizeof(resources.bulbs) / sizeof(resources.bulbs[0]);
        for (int i = 0; i < count; i++)
        {
            resources.bulbs[i] = res->bulbs[i];
        }
        count = sizeof(resources.idle) / sizeof(resources.idle[0]);
        for (int i = 0; i < count; i++)
        {
            resources.idle[i] = res->idle[i];
        }
        encode_rotary(encoder, selector);
    };

    void Controller::on_click(esphome::light::AddressableLightState *light_state,
                              esphome::rotary_encoder::RotaryEncoderSensor *encoder)
    {
        if (wake_up())
            return;

        toggle_edit();
        if (!is_edit)
        {
            encode_rotary(encoder, selector);
            return;
        }

        panel_id = (PanelID)selector.value;

        if (panel_id == EFFECT)
        {
            rotary_states[EFFECT].max_value = light_state->get_effects().size() - 1;
            encode_rotary(encoder, rotary_states[EFFECT]);
        }
        else if (panel_id < PANEL_COUNT)
        {
            encode_rotary(encoder, rotary_states[panel_id]);
        }
    }

    void Controller::on_rotate(esphome::light::AddressableLightState *light_state,
                               esphome::rotary_encoder::RotaryEncoderSensor *encoder)
    {
        if (wake_up())
            return;

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

        esphome::light::LightCall call = light_state->make_call();
        auto effects = light_state->get_effects();
        float fractional_value = static_cast<float>(index) / 100.0f;

        switch (panel_id)
        {
        case EFFECT:
            call.set_effect((index < 0 || index >= effects.size())
                                ? "None"
                                : effects[index]->get_name());
            break;
        case BRIGHTNESS:
            call.set_brightness(fractional_value);
            break;
        case RED_VALUE:
            call.set_red(fractional_value);
            break;
        case GREEN_VALUE:
            call.set_green(fractional_value);
            break;
        case BLUE_VALUE:
            call.set_blue(fractional_value);
            break;

        case HUE:
        case SATURATION:
            on_rotate_hsv(call, encoder->get_state());
            break;

        default:
            return;
        }

        call.set_state(true);
        call.perform();
    }

    void Controller::on_light_state(esphome::light::AddressableLightState *light_state)
    {
        float brightness = light_state->remote_values.get_brightness();
        float red = light_state->remote_values.get_red();
        float green = light_state->remote_values.get_green();
        float blue = light_state->remote_values.get_blue();

        rotary_states[EFFECT].value = find_effect_index(light_state);
        rotary_states[BRIGHTNESS].value = static_cast<int>(brightness * 100.0f);
        rotary_states[RED_VALUE].value = static_cast<int>(red * 100.0f);
        rotary_states[GREEN_VALUE].value = static_cast<int>(green * 100.0f);
        rotary_states[BLUE_VALUE].value = static_cast<int>(blue * 100.0f);

        glow::HSVColor hsv;
        hsv.from_rgb(glow::Color(static_cast<uint8_t>(red * 255.0f),
                                 static_cast<uint8_t>(green * 255.0f),
                                 static_cast<uint8_t>(blue * 255.0f)));

        rotary_states[HUE].value = (hsv.hue * 360) / glow::hue_limit;
        rotary_states[SATURATION].value = (hsv.saturation * 100) / glow::byte_limit;
    }

    void Controller::on_rotate_hsv(esphome::light::LightCall &call, float sensor_value)
    {
        float f_value = 255.0f;
        float f_hue = 0.0;
        float f_saturation = 0.0;

        if (HUE == panel_id)
        {
            f_hue = sensor_value;
            f_saturation = static_cast<float>(rotary_states[SATURATION].value);
        }
        else if (SATURATION == panel_id)
        {
            f_hue = static_cast<float>(rotary_states[HUE].value);
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

        if (rotary_state.wrap)
        {
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
        }

        rotary_state.value = index;

        return index;
    }

    void Controller::encode_rotary(esphome::rotary_encoder::RotaryEncoderSensor *encoder,
                                   RotaryState &rotary_state)
    {
        encoder->set_resolution(rotary_state.mode);
        if (rotary_state.wrap)
        {
            encoder->set_min_value(rotary_state.min_value - 1);
            encoder->set_max_value(rotary_state.max_value + 1);
        }
        else
        {
            encoder->set_min_value(rotary_state.min_value);
            encoder->set_max_value(rotary_state.max_value);
        }
        encoder->set_value(rotary_state.value);
    }

    int Controller::find_effect_index(esphome::light::AddressableLightState *light_state)
    {
        effect_name = light_state->get_effect_name();
        auto effects = light_state->get_effects();
        int index = 0;
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