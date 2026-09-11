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
void test_activeNeedsDoubleBuffer_false_when_single_noDB(void);
void test_activeNeedsDoubleBuffer_true_for_House(void);
void test_activeNeedsDoubleBuffer_empty_false(void);
void test_component_should_flip_only_for_DB(void);

int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_function_should_be_true);
  RUN_TEST(test_default_needsDoubleBuffer_false);
  RUN_TEST(test_house_override_true);
  RUN_TEST(test_activeNeedsDoubleBuffer_false_when_single_noDB);
  RUN_TEST(test_activeNeedsDoubleBuffer_true_for_House);
  RUN_TEST(test_activeNeedsDoubleBuffer_empty_false);
  RUN_TEST(test_component_should_flip_only_for_DB);
  return UNITY_END();
}


int main() {
  runUnityTests();
}

