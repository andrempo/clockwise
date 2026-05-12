import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display
from esphome.components import time


from esphome.const import CONF_ID

clockwise_component_ns = cg.esphome_ns.namespace('clockwise_component')
ClockwiseComponent = clockwise_component_ns.class_('ClockwiseComponent', cg.Component)

MATRIX_DISPLAY_CONFIG = 'matrix_display'
TIME_CONFIG = 'time'

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ClockwiseComponent),
    cv.Required(MATRIX_DISPLAY_CONFIG): cv.use_id(display.Display),
    cv.Required(TIME_CONFIG): cv.use_id(time.RealTimeClock),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    disp = await cg.get_variable(config[MATRIX_DISPLAY_CONFIG])
    cg.add(var.set_matrix_display(disp)) 
    time_rtc = await cg.get_variable(config[TIME_CONFIG])
    cg.add(var.set_time(time_rtc)) 
    await cg.register_component(var, config)

