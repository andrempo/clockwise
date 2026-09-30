#include "Arduino.h"
#include "SimRenderer.h"

// Locator/EventBus statics come from the real cw-gfx-engine sources,
// compiled into simcore (see ../CMakeLists.txt / ../Makefile).
unsigned long host_millis_ = 0;
HostSerial Serial;

unsigned long millis(void) { return host_millis_; }
void host_advance_millis(unsigned long step_ms) { host_millis_ += step_ms; }
