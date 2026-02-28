#include <Arduino.h>
#include <unity.h>
#include <mock_stream.cpp>
#include "communication.h"
#include "TS_lib.h"

MockStream mockStream;
Communication comm;

void setUp(void) {
  mockStream.clear();
  comm = Communication(&mockStream, nullptr, nullptr, nullptr);
}

void tearDown(void) {}

void test_status_ready(void) {
  TEST_ASSERT_TRUE(comm.isReady());
  mockStream.pushByte(0x00);
  mockStream.pushByte(0x01);
  comm.serialReceive();
  TEST_ASSERT_EQUAL_STRING("", mockStream.getTxBuffer().c_str());
  TEST_ASSERT_FALSE(comm.isReady());
}


void setup() {
  UNITY_BEGIN();

  RUN_TEST(test_status_ready);

  UNITY_END();
}

void loop() { }

