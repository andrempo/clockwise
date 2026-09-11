#include "clockface_manager.h"
#ifdef ARDUINO
#include "esphome/core/log.h"
#include <Arduino.h>
#else
#include <cstdint>
static unsigned long millis() { return 0; }
#endif

static const char* TAG = "clockwise_manager";

void ClockfaceManager::registerFace(IClockface* face, const char* name) {
    _faces.push_back({face, name});
}

void ClockfaceManager::setActive(int index) {
    if (index < 0 || index >= count()) return;
    _activeIndex = index;
    _faceNeedsSetup = true;
}

void ClockfaceManager::setRotation(bool enabled) {
    _rotationEnabled = enabled;
    _lastSwitchMs = millis();
}

void ClockfaceManager::setIntervalMs(unsigned long ms) {
    _intervalMs = ms;
}

const char* ClockfaceManager::activeName() const {
    if (_faces.empty()) return "";
    return _faces[_activeIndex].name;
}

bool ClockfaceManager::activeNeedsDoubleBuffer() const {
  if (_faces.empty()) return false;
  if (_activeIndex < 0 || _activeIndex >= (int)_faces.size()) return false;
  auto* f = _faces[_activeIndex].face;
  return f ? f->needsDoubleBuffer() : false;
}

void ClockfaceManager::init(CWDateTime* dateTime) {
    _dateTime = dateTime;
    _lastSwitchMs = millis();
}

void ClockfaceManager::update() {
    if (_faces.empty()) return;

    // Rotation: cycle to next face when interval elapses
    if (_rotationEnabled && _faces.size() > 1) {
        unsigned long now = millis();
        if (now - _lastSwitchMs >= _intervalMs) {
            _activeIndex = (_activeIndex + 1) % _faces.size();
            _lastSwitchMs = now;
            _faceNeedsSetup = true;
        }
    }

    // Call setup() on first run or after switch
    if (_faceNeedsSetup) {
        _faces[_activeIndex].face->setup(_dateTime);
        _faceNeedsSetup = false;
    }

    _faces[_activeIndex].face->update();
}
