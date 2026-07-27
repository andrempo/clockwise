// Auto-generated clockface registry implementation
#include "clockface_registry.h"
void registerFace_Pacman(ClockfaceManager& mgr, Adafruit_GFX* gfx);
void registerFace_Mario(ClockfaceManager& mgr, Adafruit_GFX* gfx);

void registerAllFaces(ClockfaceManager& mgr, Adafruit_GFX* gfx) {
    registerFace_Pacman(mgr, gfx);
    registerFace_Mario(mgr, gfx);
}
