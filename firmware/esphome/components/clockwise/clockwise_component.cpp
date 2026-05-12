#include "esphome/core/log.h"
#include "clockwise_component.h"
#include "CWDateTimeESPTime.h"


namespace esphome {
namespace clockwise_component {

static const char *TAG = "clockwise_component.component";

void ClockwiseComponent::setup() {

    gfx_wrapper = new ESPHomeGFXWrapper(matrix_display, matrix_display->get_width(), matrix_display->get_height());
    gfx_wrapper->setTextWrap(false);
    clockface = new Clockface(gfx_wrapper);
    cwDateTime = new CWDateTimeESPTime(rtc);
    
}


void ClockwiseComponent::draw() { 
    if (!initial_draw_done) {
        clockface->setup(cwDateTime);
        initial_draw_done = true;
    }
    clockface->update();

}

void ClockwiseComponent::loop() { 


}


void ClockwiseComponent::dump_config(){
    ESP_LOGCONFIG(TAG, "ClockwiseComponent");
}

}  // namespace clockwise_component
}  // namespace esphome