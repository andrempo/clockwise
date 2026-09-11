#include "unity.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_function_should_be_true(void) {
  TEST_ASSERT_TRUE(true);
}

void test_default_needsDoubleBuffer_false(void);
void test_house_override_true(void);

int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_function_should_be_true);
  RUN_TEST(test_default_needsDoubleBuffer_false);
  RUN_TEST(test_house_override_true);
  return UNITY_END();
}


int main() {
  runUnityTests();
}

