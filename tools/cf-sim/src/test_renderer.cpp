#include "SimRenderer.h"
#include <cassert>

int main() {
  SimRenderer r;
  r.drawPixel(0, 0, 0xFFFF);
  assert(r.getRaw()[0] == 0xFFFF);
  r.drawPixel(-1, 0, 0xFFFF);
  r.drawPixel(0, 64, 0xFFFF);
  r.drawPixel(64, 0, 0xFFFF);
  assert(r.getRaw()[0] == 0xFFFF);

  r.clear();
  r.fillRect(10, 10, 2, 2, 0x07E0);
  for (int y = 10; y < 12; y++)
    for (int x = 10; x < 12; x++)
      assert(r.getRaw()[y * 64 + x] == 0x07E0);

  r.clear();
  r.fillCircle(20, 20, 3, 0xF800);
  assert(r.getRaw()[20 * 64 + 20] == 0xF800);
  assert(r.getRaw()[20 * 64 + 17] == 0xF800);
  assert(r.getRaw()[20 * 64 + 16] == 0x0000);

  r.clear();
  r.fillTriangle(0, 0, 0, 5, 5, 0, 0x001F);
  assert(r.getRaw()[0 * 64 + 0] == 0x001F);

  r.clear();
  r.drawFastHLine(5, 30, 10, 0xAAAA);
  for (int x = 5; x < 15; x++) assert(r.getRaw()[30 * 64 + x] == 0xAAAA);

  return 0;
}
