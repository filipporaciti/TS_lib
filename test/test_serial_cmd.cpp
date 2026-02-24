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


void test_code_version_command(void) {
  mockStream.pushByte('Q');
  ts.update();

  TEST_ASSERT_TRUE(mockStream.getTxBuffer().equals("TSlib_11-2025"));
}


void setup() {

  UNITY_BEGIN();
  
  RUN_TEST(test_code_version_command);
  
  UNITY_END();
}

void loop() { }

