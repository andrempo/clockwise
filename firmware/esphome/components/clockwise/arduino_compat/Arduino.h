#pragma once
// Arduino compat shim for ESP-IDF builds (ESPHome esp-idf framework).
// Provides only the Arduino API surface used by cw-gfx-engine, cw-commons,
// and the clockfaces. Inactive when the Arduino framework is used (this
// directory is only on the include path for esp-idf builds).
#ifndef CLOCKWISE_ARDUINO_COMPAT_H
#define CLOCKWISE_ARDUINO_COMPAT_H

// Claim the Arduino 1.0+ API so shared engine/commons headers take their
// Arduino branches (e.g. CWDateTime.h uses ::String). ESPHome core keys off
// USE_ARDUINO (unset on esp-idf builds), so this does not affect ESPHome.
#ifndef ARDUINO
#define ARDUINO 100
#endif

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <string>
#include <algorithm>
#include "String.h"
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

#ifndef PI
#define PI 3.14159265358979323846
#endif
#define radians(deg) ((deg) * PI / 180.0)

using std::min;
using std::max;

inline unsigned long millis() {
  return (unsigned long)(esp_timer_get_time() / 1000ULL);
}

inline void delay(unsigned long ms) {
  if (ms == 0) return;
  vTaskDelay(pdMS_TO_TICKS(ms));
}

inline void randomSeed(unsigned long seed) { srand(seed); }
inline long random(long max) { return max > 0 ? (rand() % max) : 0; }
inline long random(long min, long max) {
  return min >= max ? min : (min + rand() % (max - min));
}

// Debug-output stub: faces call Serial.print/println; output is discarded.
class SerialStub {
public:
  void begin(unsigned long baud = 9600) { (void)baud; }
  size_t print(const char *s) { return s ? strlen(s) : 0; }
  size_t print(const String &s) { return s.length(); }
  size_t print(int v) { (void)v; return 1; }
  size_t print(long v) { (void)v; return 1; }
  size_t print(unsigned int v) { (void)v; return 1; }
  size_t print(unsigned long v) { (void)v; return 1; }
  size_t println(const char *s) { return s ? strlen(s) : 0; }
  size_t println(const String &s) { return s.length(); }
  size_t println(int v) { (void)v; return 1; }
  size_t println(long v) { (void)v; return 1; }
  size_t println(unsigned int v) { (void)v; return 1; }
  size_t println(unsigned long v) { (void)v; return 1; }
};
inline SerialStub Serial;

#endif  // CLOCKWISE_ARDUINO_COMPAT_H
