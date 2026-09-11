#pragma once

#include "esphome/core/component.h"
#include "esphome/components/display/display.h"
#include "esphome/components/time/real_time_clock.h"
#include "CWDateTime.h"
#include "CWDateTimeESPTime.h"
#include "clockface_manager.h"
#include "esphome/components/hub75/hub75_component.h"
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
  ClockfaceManager _manager;
  CWDateTime* _cwDateTime{nullptr};
  ESPHomeGFXWrapper* _gfxWrapper{nullptr};
  esphome::display::Display* _matrixDisplay{nullptr};
  esphome::time::RealTimeClock* _rtc{nullptr};

 public:
  ClockwiseComponent() = default;
  void setup() override;
  void loop() override;
  void draw();
  void dump_config() override;
  void set_matrix_display(esphome::display::Display* display) { _matrixDisplay = display; }
  void set_time(esphome::time::RealTimeClock* rtc_) { _rtc = rtc_; }
  ClockfaceManager& get_manager() { return _manager; }

  // Entity-facing wrappers — delegate to ClockfaceManager
  void set_active_face(int index) { _manager.setActive(index); }
  void set_rotation(bool enabled) { _manager.setRotation(enabled); }
  void set_rotation_interval(unsigned long ms) { _manager.setIntervalMs(ms); }

 private:
  bool shouldFlip() const { return _manager.activeNeedsDoubleBuffer(); }
  void tryFlip();

};

}  // namespace clockwise_component
}  // namespace esphome
