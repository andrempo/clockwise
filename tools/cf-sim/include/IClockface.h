#ifndef CF_SIM_ICLOCKFACE_H
#define CF_SIM_ICLOCKFACE_H

// Host mirror of firmware/lib/cw-commons/IClockface.h.
#include "CWDateTime.h"

class IClockface {
 public:
  virtual ~IClockface() = default;
  virtual void setup(CWDateTime* dateTime) = 0;
  virtual void update() = 0;
  virtual bool needsDoubleBuffer() const { return false; }
};

#endif
