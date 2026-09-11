#include "unity.h"
#include "IClockface.h"
#include "CWDateTime.h"

class DummyFace : public IClockface {
 public:
  void setup(CWDateTime* dt) override {}
  void update() override {}
};

void test_default_needsDoubleBuffer_false(void) {
  DummyFace f;
  TEST_ASSERT_FALSE(f.needsDoubleBuffer());
}
void test_house_override_true(void) {
  class HouseFace : public IClockface {
   public:
    void setup(CWDateTime* d) override {}
    void update() override {}
    bool needsDoubleBuffer() const override { return true; }
  };
  HouseFace h;
  TEST_ASSERT_TRUE(h.needsDoubleBuffer());
}
