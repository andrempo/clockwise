#include "SimRenderer.h"
#include "FakeDateTime.h"
#include "BMPWriter.h"
#include "Locator.h"
#include "Arduino.h"
#include "Clockface.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

// Host render-preview driver for any Clockwise clockface.
//
// The face source dir is provided at compile time (FACE_DIR include path,
// see ../CMakeLists.txt / ../Makefile); the face itself is always
// `Clockface face(display); face.setup(&dt); face.update();`, which all
// in-tree faces (house, matrix, mario, pacman) implement.

static int g_frames = 60;

static void renderHour(int hour, int minute, const char* outPath, Adafruit_GFX* display) {
  FakeDateTime dt(hour, minute);
  Clockface face(display);
  face.setup(&dt);
  host_millis_ = 0;
  for (int i = 0; i < g_frames; i++) {   // step animation deterministically
    face.update();
    host_advance_millis(1000);
  }
  SimRenderer* sr = static_cast<SimRenderer*>(display);
  writeBMP24(outPath, sr->getRaw(), 64, 64);
}

int main(int argc, char** argv) {
  static const int sweep[] = {0, 5, 6, 7, 8, 9, 12, 15, 16, 17, 19, 20, 21, 23};
  int hour = -1, min = 0;
  const char* outDir = ".";

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--time") == 0 && i + 1 < argc) {
      if (sscanf(argv[i + 1], "%d:%d", &hour, &min) < 1) { hour = -1; }
      i++;
    } else if (strcmp(argv[i], "--out") == 0 && i + 1 < argc) {
      outDir = argv[++i];
    } else if (strcmp(argv[i], "--frames") == 0 && i + 1 < argc) {
      g_frames = atoi(argv[++i]);
      if (g_frames < 1) g_frames = 1;
    }
  }

  SimRenderer renderer;
  Locator::provide(&renderer);

  if (hour >= 0) {
    char p[256];
    snprintf(p, sizeof(p), "%s/frame_%02d%02d.bmp", outDir, hour, min);
    renderHour(hour, min, p, &renderer);
    printf("wrote %s\n", p);
  } else {
    for (int h : sweep) {
      char p[256];
      snprintf(p, sizeof(p), "%s/frame_%02d.bmp", outDir, h);
      renderHour(h, 0, p, &renderer);
      printf("wrote %s\n", p);
    }
  }
  return 0;
}
