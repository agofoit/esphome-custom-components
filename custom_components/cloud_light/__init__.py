import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components.light.types import AddressableLightEffect
from esphome.components.light.effects import register_addressable_effect
from esphome.const import CONF_ID, CONF_NAME

cloud_light_ns = cg.esphome_ns.namespace("cloud_light")
YellowAddressableLightEffect = cloud_light_ns.class_(
    "YellowAddressableLightEffect", AddressableLightEffect
)
CyanAddressableLightEffect = cloud_light_ns.class_(
    "CyanAddressableLightEffect", AddressableLightEffect
)
CloudLightComponent = cloud_light_ns.class_("CloudLightComponent", cg.Component)

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
        cv.GenerateID(CONF_Yellow_ID): cv.use_id(CloudLightComponent),
    },
)
async def yellow_light_effect_to_code(config, effect_id):
    parent = await cg.get_variable(config[CONF_Yellow_ID])
    effect = cg.new_Pvariable(effect_id, config[CONF_NAME])
    return effect

@register_addressable_effect(
    "cyan",
    CyanAddressableLightEffect,
    "Cyan",
    {
        cv.GenerateID(CONF_Cyan_ID): cv.use_id(CloudLightComponent),
    },
)
async def cyan_light_effect_to_code(config, effect_id):
    parent = await cg.get_variable(config[CONF_Cyan_ID])
    effect = cg.new_Pvariable(effect_id, config[CONF_NAME])
    return effect