// Auto-generated wrapper for clockface "Mario"
// Source: /home/andre/git/cw-cf-0x01
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
namespace Mario_detail {
#include "/home/andre/git/cw-cf-0x01/Clockface.cpp"
#include "/home/andre/git/cw-cf-0x01/gfx/block.cpp"
#include "/home/andre/git/cw-cf-0x01/gfx/mario.cpp"
}  // namespace Mario_detail

class Face_Mario : public IClockface {
    Mario_detail::Clockface _impl;

public:
    Face_Mario(Adafruit_GFX* gfx) : _impl(gfx) {}
    void setup(CWDateTime* dt) override { _impl.setup(dt); }
    void update() override { _impl.update(); }
    bool needsDoubleBuffer() const override { return false; }
};

// Registration function called by clockface_registry.cpp
void registerFace_Mario(ClockfaceManager& mgr, Adafruit_GFX* gfx) {
    mgr.registerFace(new Face_Mario(gfx), "Mario");
}
