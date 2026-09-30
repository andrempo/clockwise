#include "BMPWriter.h"
#include <cstdio>
#include <cstring>

bool writeBMP24(const char* path, const uint16_t* rgb565, int w, int h) {
  if (!path || !rgb565 || w <= 0 || h <= 0) return false;
  const int rowSize = ((w * 3) + 3) & ~3;
  const int dataSize = rowSize * h;
  const int fileSize = 14 + 40 + dataSize;

  FILE* f = fopen(path, "wb");
  if (!f) return false;

  unsigned char hdr[14 + 40] = {0};
  hdr[0] = 'B'; hdr[1] = 'M';
  hdr[2] = fileSize & 0xFF; hdr[3] = (fileSize >> 8) & 0xFF;
  hdr[4] = (fileSize >> 16) & 0xFF; hdr[5] = (fileSize >> 24) & 0xFF;
  hdr[10] = 54; // pixel data offset
  hdr[14] = 40; // info header size
  hdr[18] = w & 0xFF; hdr[19] = (w >> 8) & 0xFF;
  hdr[22] = h & 0xFF; hdr[23] = (h >> 8) & 0xFF;
  hdr[26] = 1;  // planes
  hdr[28] = 24; // bpp
  hdr[34] = dataSize & 0xFF; hdr[35] = (dataSize >> 8) & 0xFF;
  hdr[36] = (dataSize >> 16) & 0xFF; hdr[37] = (dataSize >> 24) & 0xFF;
  fwrite(hdr, 1, sizeof(hdr), f);

  unsigned char pad = 0;
  for (int y = h - 1; y >= 0; y--) {          // bottom-up
    for (int x = 0; x < w; x++) {
      uint16_t px = rgb565[y * w + x];
      unsigned char r = (px >> 11) & 0x1F;
      unsigned char g = (px >> 5)  & 0x3F;
      unsigned char b = px & 0x1F;
      unsigned char r8 = (r << 3) | (r >> 2);
      unsigned char g8 = (g << 2) | (g >> 4);
      unsigned char b8 = (b << 3) | (b >> 2);
      unsigned char bgr[3] = { b8, g8, r8 };
      fwrite(bgr, 1, 3, f);
    }
    for (int i = 0; i < (rowSize - w * 3); i++) fwrite(&pad, 1, 1, f);
  }
  fclose(f);
  return true;
}
