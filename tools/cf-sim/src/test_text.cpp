#include "SimRenderer.h"
#include "picopixel.h"
#include <cassert>

int main() {
  SimRenderer r;
  r.fillScreen(0x0000);
  r.setFont(&Picopixel);
  r.setTextSize(2);
  r.setTextColor(0xFFFF);
  r.setCursor(2, 2);
  r.print("12:00");

  int lit = 0;
  for (int i = 0; i < 64 * 64; i++) if (r.getRaw()[i]) lit++;
  assert(lit > 0);

  r.fillScreen(0x0000);
  int16_t x1, y1;
  uint16_t w, h;
  r.getTextBounds("12:00", 0, 0, &x1, &y1, &w, &h);
  assert(w > 0 && h > 0);

  return 0;
}