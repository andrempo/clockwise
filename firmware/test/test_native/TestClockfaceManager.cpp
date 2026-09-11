#include "unity.h"
#include "clockface_manager.h"
#include "IClockface.h"

class FaceNoDB : public IClockface {
 public:
  void setup(CWDateTime* d) override {}
  void update() override {}
};
class FaceDB : public IClockface {
 public:
  void setup(CWDateTime* d) override {}
  void update() override {}
  bool needsDoubleBuffer() const override { return true; }
};

void test_activeNeedsDoubleBuffer_false_when_single_noDB(void) {
  ClockfaceManager m;
  FaceNoDB f;
  m.registerFace(&f, "NoDB");
  m.init(nullptr);
  TEST_ASSERT_FALSE(m.activeNeedsDoubleBuffer());
}
void test_activeNeedsDoubleBuffer_true_for_House(void) {
  ClockfaceManager m;
  FaceNoDB a;
  FaceDB h;
  m.registerFace(&a, "Mario");
  m.registerFace(&h, "House");
  m.init(nullptr);
  m.setActive(1);
  TEST_ASSERT_TRUE(m.activeNeedsDoubleBuffer());
  m.setActive(0);
  TEST_ASSERT_FALSE(m.activeNeedsDoubleBuffer());
}
void test_activeNeedsDoubleBuffer_empty_false(void) {
  ClockfaceManager m;
  TEST_ASSERT_FALSE(m.activeNeedsDoubleBuffer());
}

// Provide ClockfaceManager implementation for native (esphome component not compiled as lib)
#include "clockface_manager.cpp"
