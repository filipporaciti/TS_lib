#include <Arduino.h>
#include <unity.h>
#include <mock_stream.cpp>
#include "TS_lib.h"

MockStream mockStream;
TS_lib ts(&mockStream);

void setUp(void) {
  mockStream.clear();
}

void tearDown(void) {}

void test_code_version(void) {
  char* code_version = ts.getCodeVersion();
  TEST_ASSERT_EQUAL_STRING(DEFAULT_CODE_VERSION, code_version);
}

void setup() {
  UNITY_BEGIN();

  RUN_TEST(test_code_version);

  UNITY_END();
}

void loop() { }

