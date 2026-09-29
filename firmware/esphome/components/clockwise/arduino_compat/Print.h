#pragma once
// Minimal Arduino Print base class — just enough for Adafruit_GFX to derive
// from. Faces use draw/fill APIs, not printing, so write() discards.
#ifndef ARDUINO
#include <cstddef>
#include <cstdint>

class Print {
public:
  virtual size_t write(uint8_t b) { (void)b; return 1; }
  size_t write(const char *s) {
    size_t n = 0;
    while (s && *s++) ++n;
    return n;
  }
  size_t print(const char *s) { return write(s); }
  size_t println(const char *s) { return write(s); }
};
#endif  // ARDUINO
