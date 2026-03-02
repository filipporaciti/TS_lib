#include <Arduino.h>
#include <unity.h>
#include <mock_stream.cpp>
#include "communication.h"
#include "../mock/pages_mock.cpp"

MockStream mockStream;
PagesMock pagesMock;
Communication comm;

void setUp(void) {
  mockStream.clear();
  comm = Communication(&mockStream, (char*)"12345", nullptr, &pagesMock);
}

void tearDown(void) {}

void TEST_CMD(uint8_t* data, size_t dataSize, uint8_t* response) {
  mockStream.pushBytes(data, dataSize);
  comm.serialReceive();

  TEST_ASSERT_EQUAL_MEMORY(response, mockStream.getTxBuffer(), mockStream.getTxBufferLen());
  TEST_ASSERT_TRUE(comm.isReady());
}

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
  uint8_t data[] = {0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  // 0x82 = SERIAL_MSG_WRONG_CRC
  uint8_t response[] = {0x00, 0x01, 0x82, 0xD1, 0xB4, 0x0D, 0x81}; 

  TEST_CMD(data, sizeof(data), response);
}

void test_Q(void) {
  uint8_t data[] = {0x00, 0x01, 0x51, 0xCE, 0x6E, 0x8E, 0xEF};
  // 2 bytes for payload length, 1 byte for flag, 5 bytes for payload, 4 bytes for CRC
  uint8_t response[] = {0x00, 0x06, 0x00, 0x31, 0x32, 0x33, 0x34, 0x35, 0xBC, 0x15, 0x6C, 0xA2};

  TEST_CMD(data, sizeof(data), response);
}

void test_S(void) {
  test_Q();
}

void test_F(void) {
  uint8_t data[] = {0x00, 0x01, 0x46, 0x4D, 0xBD, 0x0B, 0x28};
  uint8_t response[] = {0x00, 0x04, 0x00, 0x30, 0x30, 0x32, 0x12, 0xBE, 0x5D, 0xFF};

  TEST_CMD(data, sizeof(data), response);
}

void test_C(void) {
  uint8_t data[] = {0x00, 0x01, 0x43, 0X3D, 0XD7, 0XFF, 0XA7};
  uint8_t response[] = {0x00, 0x02, 0x00, 0xFF, 0X6C, 0XDB, 0XFD, 0X72};

  TEST_CMD(data, sizeof(data), response);
}

void test_p(void) {
  // 1 byte for page num, 2 bytes for offset, 2 bytes for len
  uint8_t data[] = {0x00, 0x06, 0x70, 0x00, 0x00, 0x00, 0x01, 0x00, 0XA3, 0XFB, 0X9D, 0X23};
  uint8_t response[] = {0x00, 0x02, 0x00, 0x00, 0X41, 0XD9, 0X12, 0XFF};

  TEST_CMD(data, sizeof(data), response);
}

void test_p_page_out_of_range(void) {
  uint8_t data[] = {0x00, 0x06, 0x70, 0xFF, 0x00, 0x00, 0x01, 0x00, 0X69, 0X9F, 0X5A, 0XAF};
  uint8_t response[] = {0x00, 0x01, 0x84, 0x38, 0xD7, 0xA8, 0xB4}; // 0x84 = SERIAL_MSG_RANGE_ERR

  TEST_CMD(data, sizeof(data), response);
}

void test_p_offset_out_of_range(void) {
  uint8_t data[] = {0x00, 0x06, 0x70, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0XC3, 0X66, 0XAF, 0X3F};
  uint8_t response[] = {0x00, 0x01, 0x84, 0x38, 0xD7, 0xA8, 0xB4}; // 0x84 = SERIAL_MSG_RANGE_ERR

  TEST_CMD(data, sizeof(data), response);
}


void setup() {
  UNITY_BEGIN();

  RUN_TEST(test_status_ready);
  RUN_TEST(test_rx_buffer_overflow);
  RUN_TEST(test_wrong_crc);
  RUN_TEST(test_Q);
  RUN_TEST(test_S);
  RUN_TEST(test_F);
  RUN_TEST(test_C);
  RUN_TEST(test_p);
  RUN_TEST(test_p_page_out_of_range);
  RUN_TEST(test_p_offset_out_of_range);

  UNITY_END();
}

void loop() { }

