#pragma once
// Minimal Arduino Print base class — just enough for Adafruit_GFX to derive
// from. Faces use draw/fill APIs, not printing, so write() discards.
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
    while (s && *s++) ++n;
    return n;
  }
  size_t print(const char *s) { return write(s); }
  size_t print(const String &s) { return write(s.c_str()); }
  size_t print(int v) { (void)v; return 1; }
  size_t print(long v) { (void)v; return 1; }
  size_t print(unsigned int v) { (void)v; return 1; }
  size_t print(unsigned long v) { (void)v; return 1; }
  size_t println(const char *s) { return write(s); }
  size_t println(const String &s) { return write(s.c_str()); }
  size_t println(int v) { (void)v; return 1; }
  size_t println(long v) { (void)v; return 1; }
  size_t println(unsigned int v) { (void)v; return 1; }
  size_t println(unsigned long v) { (void)v; return 1; }
};

#endif  // CLOCKWISE_PRINT_H
