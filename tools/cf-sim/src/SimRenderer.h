#ifndef HOST_SIMRENDERER_H
#define HOST_SIMRENDERER_H
#include "Adafruit_GFX.h"
#include <cstdint>
#include <cstring>

class SimRenderer : public Adafruit_GFX {
  uint16_t buf[64 * 64];
public:
  SimRenderer() : Adafruit_GFX(64, 64) { clear(); }
  void clear() { memset(buf, 0, sizeof(buf)); }
  void drawPixel(int16_t x, int16_t y, uint16_t c) override;
  const uint16_t* getRaw() const { return buf; }
};
#endif
