// Host-only supplements for cw-gfx-engine.
//
// Sprite declares `virtual const char* name()` but no base definition
// exists in-tree (faces override it; the device link evidently tolerates
// that). Provide a weak default so host links succeed – face overrides
// still win.
#include "Sprite.h"

__attribute__((weak)) const char* Sprite::name() { return "sprite"; }
