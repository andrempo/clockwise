#pragma once
// Minimal Arduino Print base class — just enough for Adafruit_GFX to derive
// from. All output routes through virtual write(uint8_t); Adafruit_GFX
// overrides it to draw glyphs, so write/print/println forward per byte.
#ifndef CLOCKWISE_PRINT_H
#define CLOCKWISE_PRINT_H

#include <cstddef>
#include <cstdint>
#include "String.h"

class Print {
public:
  virtual size_t write(uint8_t b) { (void)b; return 1; }
  size_t write(const char *s) {
    size_t n = 0;
    while (s && *s) n += write((uint8_t)*s++);
    return n;
  }
  size_t print(const char *s) { return write(s); }
  size_t print(const String &s) { return write(s.c_str()); }
  size_t print(int v) { return write(std::to_string(v).c_str()); }
  size_t print(long v) { return write(std::to_string(v).c_str()); }
  size_t print(unsigned int v) { return write(std::to_string(v).c_str()); }
  size_t print(unsigned long v) { return write(std::to_string(v).c_str()); }
  size_t println(const char *s) {
    size_t n = print(s);
    n += write((uint8_t)'\r');
    n += write((uint8_t)'\n');
    return n;
  }
  size_t println(const String &s) {
    size_t n = print(s);
    n += write((uint8_t)'\r');
    n += write((uint8_t)'\n');
    return n;
  }
  size_t println(int v) {
    size_t n = print(v);
    n += write((uint8_t)'\r');
    n += write((uint8_t)'\n');
    return n;
  }
  size_t println(long v) {
    size_t n = print(v);
    n += write((uint8_t)'\r');
    n += write((uint8_t)'\n');
    return n;
  }
  size_t println(unsigned int v) {
    size_t n = print(v);
    n += write((uint8_t)'\r');
    n += write((uint8_t)'\n');
    return n;
  }
  size_t println(unsigned long v) {
    size_t n = print(v);
    n += write((uint8_t)'\r');
    n += write((uint8_t)'\n');
    return n;
  }
};

#endif  // CLOCKWISE_PRINT_H
