#include <Arduino.h>
#include <unity.h>
#include <mock_stream.cpp>
#include "communication.h"
#include "TS_lib.h"

MockStream mockStream;
Communication comm;

void setUp(void) {
  mockStream.clear();
  comm = Communication(&mockStream, (char*)"12345", nullptr, nullptr);
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

void test_wrong_crc(void) {
  uint8_t data[7] = {0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  mockStream.pushBytes(data, sizeof(data));
  comm.serialReceive();

  // 0x82 = SERIAL_MSG_WRONG_CRC
  TEST_ASSERT_EQUAL_MEMORY("\x00\x01\x82\xD1\xB4\x0D\x81", mockStream.getTxBuffer(), 7);
  TEST_ASSERT_TRUE(comm.isReady());
}

void test_Q(void) {
  uint8_t data[7] = {0x00, 0x01, 0x51, 0xCE, 0x6E, 0x8E, 0xEF};
  mockStream.pushBytes(data, sizeof(data));
  comm.serialReceive();

  // 2 bytes for payload length, 1 byte for flag, 5 bytes for payload, 4 bytes for CRC
  uint8_t response[] = {0x00, 0x06, 0x00, 0x31, 0x32, 0x33, 0x34, 0x35, 0xBC, 0x15, 0x6C, 0xA2};
  TEST_ASSERT_EQUAL_MEMORY(response, mockStream.getTxBuffer(), mockStream.getTxBufferLen());
  TEST_ASSERT_TRUE(comm.isReady());
}

void test_S(void) {
  test_Q();
}

void test_F(void) {
  uint8_t data[7] = {0x00, 0x01, 0x46, 0x4D, 0xBD, 0x0B, 0x28};
  mockStream.pushBytes(data, sizeof(data));
  comm.serialReceive();

  uint8_t response[] = {0x00, 0x04, 0x00, 0x30, 0x30, 0x32, 0x12, 0xBE, 0x5D, 0xFF};
  TEST_ASSERT_EQUAL_MEMORY(response, mockStream.getTxBuffer(), mockStream.getTxBufferLen());
  TEST_ASSERT_TRUE(comm.isReady());
}


void setup() {
  UNITY_BEGIN();

  RUN_TEST(test_status_ready);
  RUN_TEST(test_rx_buffer_overflow);
  RUN_TEST(test_wrong_crc);
  RUN_TEST(test_Q);
  RUN_TEST(test_S);
  RUN_TEST(test_F);

  UNITY_END();
}

void loop() { }

