#include <Arduino.h>
#include <unity.h>
#include <mock_stream.cpp>
#include "communication_legacy.h"
#include "TS_lib.h"

MockStream mockStream;
Communication_legacy comm(&mockStream, DEFAULT_CODE_VERSION);

void setUp(void) {
  mockStream.clear();
}

void tearDown(void) {}


void test_code_version_command(void) {
  mockStream.pushByte('Q');
  comm.serialReceiveLegacy();

  TEST_ASSERT_TRUE(mockStream.getTxBuffer().equals(DEFAULT_CODE_VERSION));
}

void test_code_version2_command(void) {
  mockStream.pushByte('S');
  comm.serialReceiveLegacy();

  TEST_ASSERT_TRUE(mockStream.getTxBuffer().equals(DEFAULT_CODE_VERSION));
}


void setup() {

  UNITY_BEGIN();
  
  RUN_TEST(test_code_version_command);
  RUN_TEST(test_code_version2_command);
  
  UNITY_END();
}

void loop() { }

