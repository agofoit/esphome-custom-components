import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import light
from esphome.const import (
    CONF_CHANNEL,
    CONF_CLOCK_PIN,
    CONF_DATA_PIN,
    CONF_METHOD,
    CONF_NUM_LEDS,
    CONF_PIN,
    CONF_TYPE,
    CONF_VARIANT,
    CONF_OUTPUT_ID,
    CONF_INVERT,
)
from esphome.components.esp32 import get_esp32_variant
from esphome.components.esp32.const import (
    VARIANT_ESP32,
    VARIANT_ESP32C3,
    VARIANT_ESP32S2,
    VARIANT_ESP32S3,
)
from esphome.core import CORE
from dataclasses import dataclass
from typing import Any

CHANNEL_DYNAMIC = "dynamic"

custom_neo_ns = cg.esphome_ns.namespace("custom_neo")
NeoPixelBusLightOutputBase = custom_neo_ns.class_(
    "NeoPixelBusLightOutputBase", light.AddressableLight
)
NeoPixelRGBLightOutput = custom_neo_ns.class_(
    "NeoPixelRGBLightOutput", NeoPixelBusLightOutputBase
)
NeoPixelRGBWLightOutput = custom_neo_ns.class_(
    "NeoPixelRGBWLightOutput", NeoPixelBusLightOutputBase
)
ESPNeoPixelOrder = custom_neo_ns.namespace("ESPNeoPixelOrder")
NeoRgbFeature = cg.global_ns.NeoRgbFeature
NeoRgbwFeature = cg.global_ns.NeoRgbwFeature

def validate_type(value):
    value = cv.string(value).upper()
    if "R" not in value:
        raise cv.Invalid("Must have R in type")
    if "G" not in value:
        raise cv.Invalid("Must have G in type")
    if "B" not in value:
        raise cv.Invalid("Must have B in type")
    rest = set(value) - set("RGBW")
    if rest:
        raise cv.Invalid(f"Type has invalid color: {', '.join(rest)}")
    if len(set(value)) != len(value):
        raise cv.Invalid("Type has duplicate color!")
    return value

def _validate(config):
    variant = config[CONF_VARIANT]
    return config

def _validate_method(value):
    if value is None:
        # default method is determined afterwards because it depends on the chip type chosen
        return None

    compat_methods = {}

    for channel in range(8):
        compat_methods[f"ESP32_RMT_{channel}"] = {
            CONF_TYPE: "esp32_rmt",
            CONF_CHANNEL: channel,
        }

    if isinstance(value, str):
        if value.upper() in compat_methods:
            return _validate_method(compat_methods[value.upper()])
        return _validate_method({CONF_TYPE: value})
    return cv.typed_schema(
        {k: v.method_schema for k, v in METHODS.items()}, lower=True
    )(value)

CONFIG_SCHEMA = cv.All(
    cv.only_with_arduino,
    cv.require_framework_version(esp32_arduino=cv.Version(0, 0, 0)),
    light.ADDRESSABLE_LIGHT_SCHEMA.extend(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(NeoPixelBusLightOutputBase),
            cv.Optional(CONF_TYPE, default="GRB"): validate_type,
            cv.Required(CONF_VARIANT): cv.one_of("ws2812x", lower=True),
            cv.Optional(CONF_METHOD, default="esp32_rmt"): _validate_method,
            cv.Optional(CONF_INVERT, default="no"): cv.boolean,
            cv.Optional(CONF_PIN): pins.internal_gpio_output_pin_number,
            cv.Optional(CONF_CLOCK_PIN): pins.internal_gpio_output_pin_number,
            cv.Optional(CONF_DATA_PIN): pins.internal_gpio_output_pin_number,
            cv.Required(CONF_NUM_LEDS): cv.positive_not_null_int,
        }
    ).extend(cv.COMPONENT_SCHEMA),
    _validate,
)

def _esp32_rmt_default_channel():
    return {
        VARIANT_ESP32C3: 1,
        VARIANT_ESP32S2: 1,
        VARIANT_ESP32S3: 1,
    }.get(get_esp32_variant(), 6)

def _validate_esp32_rmt_channel(value):
    if isinstance(value, str) and value.lower() == CHANNEL_DYNAMIC:
        value = CHANNEL_DYNAMIC
    else:
        value = cv.int_(value)
    variant_channels = {
        VARIANT_ESP32: [0, 1, 2, 3, 4, 5, 6, 7, CHANNEL_DYNAMIC],
        VARIANT_ESP32C3: [0, 1, CHANNEL_DYNAMIC],
        VARIANT_ESP32S2: [0, 1, 2, 3, CHANNEL_DYNAMIC],
        VARIANT_ESP32S3: [0, 1, 2, 3, CHANNEL_DYNAMIC],
    }
    variant = get_esp32_variant()
    if variant not in variant_channels:
        raise cv.Invalid(f"{variant} does not support the rmt method")
    if value not in variant_channels[variant]:
        raise cv.Invalid(f"{variant} does not support rmt channel {value}")
    return value

neo_ns = cg.global_ns

def _esp32_rmt_to_code(config, chip: str, inverted: bool):
    # https://github.com/Makuna/NeoPixelBus/blob/master/src/internal/NeoEsp32RmtMethod.h
    channel = {
        0: neo_ns.NeoEsp32RmtChannel0,
        1: neo_ns.NeoEsp32RmtChannel1,
        2: neo_ns.NeoEsp32RmtChannel2,
        3: neo_ns.NeoEsp32RmtChannel3,
        4: neo_ns.NeoEsp32RmtChannel4,
        5: neo_ns.NeoEsp32RmtChannel5,
        6: neo_ns.NeoEsp32RmtChannel6,
        7: neo_ns.NeoEsp32RmtChannel7,
        CHANNEL_DYNAMIC: neo_ns.NeoEsp32RmtChannelN,
    }[config[CONF_CHANNEL]]
    lookup = {
        ("ws2812x", False): neo_ns.NeoEsp32RmtSpeedWs2812x,
        ("ws2812x", True): neo_ns.NeoEsp32RmtInvertedSpeedWs2812x,
    }
    speed = lookup[("ws2812x", inverted)]
    return neo_ns.NeoEsp32RmtMethodBase.template(speed, channel)

@dataclass
class MethodDescriptor:
    method_schema: Any
    to_code: Any
    supported_chips: list[str]
    extra_validate: Any = None

METHODS = {
    "esp32_rmt": MethodDescriptor(
        method_schema=cv.All(
            cv.only_on_esp32,
            {
                cv.Optional(
                    CONF_CHANNEL, default=_esp32_rmt_default_channel
                ): _validate_esp32_rmt_channel,
            },
        ),
        to_code=_esp32_rmt_to_code,
        supported_chips=["ws2812x"],
    ),
}

async def to_code(config):
    has_white = "W" in config[CONF_TYPE]
    method = config[CONF_METHOD]

    method_template = METHODS[method[CONF_TYPE]].to_code(
        method, config[CONF_VARIANT], config[CONF_INVERT]
    )

    if has_white:
        out_type = NeoPixelRGBWLightOutput.template(method_template)
    else:
        out_type = NeoPixelRGBLightOutput.template(method_template)
    rhs = out_type.new()
    var = cg.Pvariable(config[CONF_OUTPUT_ID], rhs, out_type)
    await light.register_light(var, config)
    await cg.register_component(var, config)

    cg.add(var.add_leds(config[CONF_NUM_LEDS], config[CONF_PIN]))

    cg.add(var.set_pixel_order(getattr(ESPNeoPixelOrder, config[CONF_TYPE])))

    # https://github.com/Makuna/NeoPixelBus/blob/master/library.json
    # Version Listed Here: https://registry.platformio.org/libraries/makuna/NeoPixelBus/versions
    cg.add_library("makuna/NeoPixelBus", "2.7.3")
