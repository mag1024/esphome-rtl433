import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import mqtt
from esphome import const, automation
from esphome.const import (
    CONF_ID,
    CONF_MQTT_ID,
    CONF_TOPIC_PREFIX,
    CONF_SOURCE,
)

CODEOWNERS = ["@mag1024"]
DEPENDENCIES = ["mqtt"]

rtl433_mqtt_ns = cg.esphome_ns.namespace("rtl433_mqtt")
Gateway = rtl433_mqtt_ns.class_("Gateway", cg.Component)

StatusAction = rtl433_mqtt_ns.class_("StatusAction", automation.Action)
StopAction = rtl433_mqtt_ns.class_("StopAction", automation.Action)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Gateway),
            cv.GenerateID(CONF_MQTT_ID): cv.use_id(mqtt.MQTTClientComponent),
            cv.Optional(CONF_TOPIC_PREFIX, default="rtl_433"): cv.string,
            cv.Optional(CONF_SOURCE, default=""): cv.string,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)
GATEWAY_ID_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(Gateway),
    }
)

async def to_code(config):
    mqtt_client = await cg.get_variable(config[CONF_MQTT_ID])
    var = cg.new_Pvariable(config[CONF_ID], mqtt_client, config[CONF_TOPIC_PREFIX], config[CONF_SOURCE])
    await cg.register_component(var, config)

@automation.register_action("rtl433_mqtt.status", StatusAction,
                            automation.maybe_simple_id(GATEWAY_ID_SCHEMA))
@automation.register_action("rtl433_mqtt.stop", StopAction,
                            automation.maybe_simple_id(GATEWAY_ID_SCHEMA))

async def action_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    return var