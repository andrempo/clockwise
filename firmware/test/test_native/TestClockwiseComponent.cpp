#include "unity.h"
#include "clockface_manager.h"
#include "IClockface.h"

class FaceNoDB2 : public IClockface {
 public:
  void setup(CWDateTime* d) override {}
  void update() override {}
};
class FaceDB2 : public IClockface {
 public:
  void setup(CWDateTime* d) override {}
  void update() override {}
  bool needsDoubleBuffer() const override { return true; }
};

void test_component_should_flip_only_for_DB(void) {
  ClockfaceManager m;
  FaceNoDB2 a;
  FaceDB2 h;
  m.registerFace(&a, "A");
  m.registerFace(&h, "H");
  m.init(nullptr);
  // Simulate ClockwiseComponent::shouldFlip logic
  m.setActive(0);
  TEST_ASSERT_FALSE(m.activeNeedsDoubleBuffer());
  m.setActive(1);
  TEST_ASSERT_TRUE(m.activeNeedsDoubleBuffer());
}
