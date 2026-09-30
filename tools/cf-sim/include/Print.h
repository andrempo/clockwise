#ifndef HOST_PRINT_H
#define HOST_PRINT_H

#include <cstddef>
#include <cstdint>
#include <string>

class Print {
public:
  virtual ~Print() = default;
  virtual size_t write(uint8_t c) = 0;
  size_t print(const char* s) {
    size_t n = 0;
    if (s) { while (*s) { write((uint8_t)*s++); n++; } }
    return n;
  }
  // Arduino also accepts String; our host String derives from std::string.
  size_t print(const std::string& s) { return print(s.c_str()); }
  size_t print(long v) {
    char b[24];
    snprintf(b, sizeof(b), "%ld", v);
    return print(b);
  }
  size_t print(int v) { return print((long)v); }
};

#endif
