#pragma once

#include <vector>
#include "IClockface.h"

class ClockfaceManager {
public:
    struct Entry {
        IClockface* face;
        const char* name;
    };

    void registerFace(IClockface* face, const char* name);
    void setActive(int index);
    void setRotation(bool enabled);
    void setIntervalMs(unsigned long ms);

    int activeIndex() const { return _activeIndex; }
    bool rotationEnabled() const { return _rotationEnabled; }
    unsigned long intervalMs() const { return _intervalMs; }
    int count() const { return static_cast<int>(_faces.size()); }
    const char* activeName() const;
    bool activeNeedsDoubleBuffer() const;

    void init(CWDateTime* dateTime);
    void update();

private:
    std::vector<Entry> _faces;
    int _activeIndex = 0;
    bool _rotationEnabled = false;
    unsigned long _intervalMs = 10000;
    unsigned long _lastSwitchMs = 0;
    bool _faceNeedsSetup = true;
    CWDateTime* _dateTime = nullptr;
};
