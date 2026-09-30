#ifndef CF_SIM_ARDUINO_H
#define CF_SIM_ARDUINO_H

// Host stub for Arduino.h – just enough for clockfaces + cw-gfx-engine
// headers to compile on Linux. Not a full Arduino emulation.

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cmath>
#include <string>
#include <algorithm>
#include "Print.h"

#define PROGMEM
#define pgm_read_byte(addr)   (*(const uint8_t*)(addr))
#define pgm_read_word(addr)   (*(const uint16_t*)(addr))
#define pgm_read_dword(addr)  (*(const uint32_t*)(addr))

#ifndef radians
#define radians(x) ((x) * 0.017453292519943295f)
#endif

typedef bool boolean;
typedef uint8_t byte;

// Arduino min/max (macros on device); using-declarations are enough on host.
using std::min;
using std::max;

// Minimal Arduino::String shape on top of std::string – just what the
// faces use (default/int/cstr construction, copy). Not a full emulation.
struct String : public std::string {
  String() : std::string() {}
  String(const char* s) : std::string(s ? s : "") {}
  String(int v) : std::string(std::to_string(v)) {}
  String(const std::string& s) : std::string(s) {}
};

unsigned long millis(void);
extern unsigned long host_millis_;
void host_advance_millis(unsigned long step_ms);

// Deterministic Arduino random() on host (seeded via randomSeed).
inline void randomSeed(unsigned long seed) { srand((unsigned)seed); }
inline long random(long max) { return max > 0 ? (long)(rand() % max) : 0; }
inline long random(long min, long max) {
  return min >= max ? min : min + (long)(rand() % (max - min));
}

// Minimal Serial stub (log to stdout, enough to link faces using Serial).
struct HostSerial {
  void print(const char* s) { fputs(s ? s : "", stdout); }
  void print(int v) { printf("%d", v); }
  void println(const char* s) { puts(s ? s : ""); }
  void println(int v) { printf("%d\n", v); }
};
extern HostSerial Serial;

#endif
