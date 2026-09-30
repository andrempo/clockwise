#include "SimRenderer.h"
void SimRenderer::drawPixel(int16_t x, int16_t y, uint16_t c) {
  if (x < 0 || y < 0 || x >= 64 || y >= 64) return;
  buf[y * 64 + x] = c;
}
