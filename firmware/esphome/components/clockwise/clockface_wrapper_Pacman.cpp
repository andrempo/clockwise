// Auto-generated wrapper for clockface "Pacman"
// Source: /home/andre/git/cw-cf-0x05
#include "IClockface.h"
#include "CWDateTime.h"
#include "clockface_manager.h"
#include <Adafruit_GFX.h>

// Pre-include shared library headers so they remain in global namespace.
// Include guards skip them when clockface code includes them inside the unique namespace below.
#include <Arduino.h>
#include <functional>
#include <Object.h>
#include <Locator.h>
#include <EventBus.h>
#include <EventTask.h>
#include <Game.h>
#include <Sprite.h>
#include <Tile.h>
#include <Macros.h>
#include <ColorUtil.h>
#include <ImageUtils.h>

// Unique namespace prevents linker collision on class Clockface
// when multiple wrappers are compiled in the same project.
namespace Pacman_detail {
#include "/home/andre/git/cw-cf-0x05/Clockface.cpp"
#include "/home/andre/git/cw-cf-0x05/clockface_ai.cpp"
#include "/home/andre/git/cw-cf-0x05/clockface_map.cpp"
#include "/home/andre/git/cw-cf-0x05/clockface_pathfinding.cpp"
#include "/home/andre/git/cw-cf-0x05/entity.cpp"
#include "/home/andre/git/cw-cf-0x05/ghost.cpp"
#include "/home/andre/git/cw-cf-0x05/pacman.cpp"
}  // namespace Pacman_detail

class Face_Pacman : public IClockface {
    Pacman_detail::Clockface _impl;

public:
    Face_Pacman(Adafruit_GFX* gfx) : _impl(gfx) {}
    void setup(CWDateTime* dt) override { _impl.setup(dt); }
    void update() override { _impl.update(); }
    bool needsDoubleBuffer() const override { return false; }
};

// Registration function called by clockface_registry.cpp
void registerFace_Pacman(ClockfaceManager& mgr, Adafruit_GFX* gfx) {
    mgr.registerFace(new Face_Pacman(gfx), "Pacman");
}
