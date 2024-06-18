import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.const import CONF_ID

DEPENDENCIES = ["light"]

red_green_blink_effect_ns = cg.esphome_ns.namespace("red_green_blink_effect")
RedGreenBlinkEffect = red_green_blink_effect_ns.class_(
    "RedGreenBlinkEffect", light.AddressableEffect, cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(RedGreenBlinkEffect),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    return var