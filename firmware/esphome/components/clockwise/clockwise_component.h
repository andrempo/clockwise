#pragma once

#include "esphome/core/component.h"
#include "esphome/components/display/display.h"
#include "esphome/components/time/real_time_clock.h"
#include "Clockface.h"
#include <Adafruit_GFX.h>

namespace esphome {
namespace clockwise_component {

class ESPHomeGFXWrapper : public Adafruit_GFX {
private:
    esphome::display::Display* display_;
public:
    ESPHomeGFXWrapper(esphome::display::Display* display, int16_t w, int16_t h) 
        : Adafruit_GFX(w, h), display_(display) {}

    void drawPixel(int16_t x, int16_t y, uint16_t color) override {
        // Unpack RGB565 and draw using ESPHome's Display API
        uint8_t r = (color >> 11) & 0x1F;
        uint8_t g = (color >> 5) & 0x3F;
        uint8_t b = color & 0x1F;

        // Scale 5-bit and 6-bit colors to 8-bit
        r = (r * 255) / 31;
        g = (g * 255) / 63;
        b = (b * 255) / 31;

        if (display_) {
            display_->draw_pixel_at(x, y, esphome::Color(r, g, b));
        }
    }
};

class ClockwiseComponent : public Component {
 private:
  Clockface* clockface;
  CWDateTime* cwDateTime;
  ESPHomeGFXWrapper* gfx_wrapper;
  esphome::display::Display *matrix_display;
  esphome::time::RealTimeClock *rtc;
  uint32_t lastMillisTime = 0;
  bool initial_draw_done = false;

 public:
  ClockwiseComponent() {
  }
  void setup() override;
  void loop() override;
  void draw();
  void dump_config() override;
  void set_matrix_display(esphome::display::Display* display) { matrix_display = display; }
  void set_time(esphome::time::RealTimeClock* rtc_) { rtc = rtc_; }
  
};

}  // namespace clockwise_component
}  // namespace esphome
