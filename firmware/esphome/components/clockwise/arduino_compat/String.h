#pragma once
// Minimal Arduino String for ESP-IDF builds (ESPHome esp-idf framework).
// Covers exactly the Arduino String API surface the clockfaces use; anything
// else must be added here only when the compile log names it.
#ifndef CLOCKWISE_STRING_H
#define CLOCKWISE_STRING_H

#include <string>

// Flash-string helper: F() collapses to a plain string (no PROGMEM strings
// on ESP-IDF); the type exists so Adafruit_GFX declarations resolve.
struct __FlashStringHelper {};
#define F(s) (s)

class String : public std::string {
public:
  using std::string::string;
  String(int v) : std::string(std::to_string(v)) {}
  String(long v) : std::string(std::to_string(v)) {}
  String(unsigned int v) : std::string(std::to_string(v)) {}
  String(unsigned long v) : std::string(std::to_string(v)) {}
};

#endif  // CLOCKWISE_STRING_H
