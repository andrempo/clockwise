#pragma once
// Arduino compat shim for ESP-IDF builds (ESPHome esp-idf framework).
// Provides only the Arduino API surface used by cw-gfx-engine, cw-commons,
// and the clockfaces. Inactive when the Arduino framework is used.
#ifndef ARDUINO
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <algorithm>
#include "Print.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PROGMEM
#define pgm_read_byte(addr) (*(const uint8_t *)(addr))
#define pgm_read_word(addr) (*(const uint16_t *)(addr))
#define pgm_read_dword(addr) (*(const uint32_t *)(addr))

typedef bool boolean;
typedef uint8_t byte;

using std::min;
using std::max;

inline unsigned long millis() {
  return (unsigned long)(esp_timer_get_time() / 1000ULL);
}

inline void delay(unsigned long ms) {
  if (ms == 0) return;
  vTaskDelay(pdMS_TO_TICKS(ms));
}
#endif  // ARDUINO
