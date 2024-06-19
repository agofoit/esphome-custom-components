import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components.light.types import AddressableLightEffect
from esphome.components.light.effects import register_addressable_effect
from esphome.const import CONF_ID, CONF_NAME, CONF_CHANNELS

AUTO_LOAD = ["socket"]
DEPENDENCIES = ["network"]

cloud_light_ns = cg.esphome_ns.namespace("cloud_light")
YellowAddressableLightEffect = cloud_light_ns.class_(
    "YellowAddressableLightEffect", AddressableLightEffect
)
CyanAddressableLightEffect = cloud_light_ns.class_(
    "CyanAddressableLightEffect", AddressableLightEffect
)
CloudLightComponent = cloud_light_ns.class_("CloudLightComponent", cg.Component)

CHANNELS = {
    "MONO": cloud_light_ns.CloudLight_MONO,
    "RGB": cloud_light_ns.CloudLight_RGB,
    "RGBW": cloud_light_ns.CloudLight_RGBW,
}

CONF_Yellow_ID = "yellow_id"
CONF_Cyan_ID = "cyan_id"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(CloudLightComponent),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)


@register_addressable_effect(
    "yellow",
    YellowAddressableLightEffect,
    "Yellow",
    {
        cv.GenerateID(CONF_Yellow_ID): cv.use_id(CloudLightComponent),  # Reuse the existing component
        cv.Optional(CONF_CHANNELS, default="RGB"): cv.one_of(*CHANNELS, upper=True),
    },
)
async def yellow_light_effect_to_code(config, effect_id):
    parent = await cg.get_variable(config[CONF_Yellow_ID])

    effect = cg.new_Pvariable(effect_id, config[CONF_NAME])
    cg.add(effect.set_yellow(parent))  # Make sure this method exists in your effect class
    return effect

@register_addressable_effect(
    "cyan",
    CyanAddressableLightEffect,
    "Cyan",
    {
        cv.GenerateID(CONF_Cyan_ID): cv.use_id(CloudLightComponent),  # Reuse the existing component
        cv.Optional(CONF_CHANNELS, default="RGB"): cv.one_of(*CHANNELS, upper=True),
    },
)
async def cyan_light_effect_to_code(config, effect_id):
    parent = await cg.get_variable(config[CONF_Cyan_ID])

    effect = cg.new_Pvariable(effect_id, config[CONF_NAME])
    cg.add(effect.set_cyan(parent))  # Make sure this method exists in your effect class
    return effect