import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import (
    CONF_ID,
    CONF_DISTANCE,
    CONF_POWER,
    STATE_CLASS_MEASUREMENT,
    DEVICE_CLASS_DISTANCE,
    UNIT_CENTIMETER,
    UNIT_EMPTY,
    ICON_RULER,
    ICON_POWER
)

DEPENDENCIES = ["uart"]

tfmini_ns = cg.esphome_ns.namespace("tfmini")
TFMiniSensor = tfmini_ns.class_("TFMiniSensor", cg.PollingComponent, uart.UARTDevice)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(TFMiniSensor),
            cv.Optional(CONF_DISTANCE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CENTIMETER,
                icon=ICON_RULER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_DISTANCE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_POWER): sensor.sensor_schema(
                unit_of_measurement=UNIT_EMPTY,
                icon=ICON_POWER,
                accuracy_decimals=0,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(cv.polling_component_schema("0.01s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_DISTANCE in config:
        sens = await sensor.new_sensor(config[CONF_DISTANCE])
        cg.add(var.set_distance_sensor(sens))
    if CONF_POWER in config:
        sens = await sensor.new_sensor(config[CONF_POWER])
        cg.add(var.set_strength_sensor(sens))