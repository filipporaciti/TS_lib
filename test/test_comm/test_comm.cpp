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
  TEST_ASSERT_EQUAL_STRING("", mockStream.getTxBuffer());
  TEST_ASSERT_FALSE(comm.isReady());
}

void test_rx_buffer_overflow(void) {
  mockStream.pushByte(0xFF);
  mockStream.pushByte(0xFF);
  comm.serialReceive();

  // 0x84 = SERIAL_MSG_RANGE_ERR
  // 2 bytes for payload length, 1 byte for flag, 4 bytes for CRC
  TEST_ASSERT_EQUAL_MEMORY("\x00\x01\x84\x38\xD7\xA8\xB4", mockStream.getTxBuffer(), 7);
  TEST_ASSERT_TRUE(comm.isReady());
  TEST_ASSERT_EQUAL(0, mockStream.available());
}


void setup() {
  UNITY_BEGIN();

  RUN_TEST(test_status_ready);
  RUN_TEST(test_rx_buffer_overflow);

  UNITY_END();
}

void loop() { }

