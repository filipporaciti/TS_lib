#include <Arduino.h>
#include <unity.h>
#include <mock_stream.cpp>
#include "communication.h"
#include "../mock/pages_mock.cpp"
#include "../mock/rt_data_mock.cpp"

MockStream mockStream;
PagesMock pagesMock;
RtDataMock rtDataMock;
Communication comm;

uint8_t range_err_resp[] = {0x00, 0x01, 0x84, 0x38, 0xD7, 0xA8, 0xB4}; // 0x84 = SERIAL_MSG_RANGE_ERR

void setUp(void) {
  mockStream.clear();
  pagesMock.reset();
  comm = Communication(&mockStream, (char*)"12345", &rtDataMock, &pagesMock);
}

void tearDown(void) {}

void TEST_CMD(uint8_t* data, size_t dataSize, uint8_t* response) {
  mockStream.pushBytes(data, dataSize);
  comm.serialReceive();

  TEST_ASSERT_EQUAL_MEMORY(response, mockStream.getTxBuffer(), mockStream.getTxBufferLen());
  TEST_ASSERT_TRUE(comm.isReady());
}

void test_f(void) {
  uint8_t data[] = {0x00, 0x02, 0x66, 0x01, 0X05, 0X79, 0XEE, 0X48};
  uint8_t response[] = {0x00, 0x06, 0x00, 0x02, 0x00, 0xF0, 0x00, 0xF0, 0XC3, 0X7B, 0X73, 0X0F};

  TEST_CMD(data, sizeof(data), response);
}

void test_unknown_command(void) {
  uint8_t data[] = {0x00, 0x01, 0xFF, 0XFF, 0X00, 0X00, 0X00};
  uint8_t response[] = {0x00, 0x01, 0x83, 0XA6, 0XB3, 0X3D, 0X17};

  TEST_CMD(data, sizeof(data), response);
}

void test_M_page_out_of_range(void) {
  // 1 byte for page num, 2 bytes for offset, 2 bytes for len, n bytes for values
  uint8_t data[] = {0x00, 0x08, 0x4D, 0x01, 0x00, 0x00, 0x02, 0x00, 0x0B, 0x0A, 0XD8, 0XE5, 0XFB, 0XBF};

  TEST_CMD(data, sizeof(data), range_err_resp);
}

void test_M_offset_out_of_range(void) {
  // 1 byte for page num, 2 bytes for offset, 2 bytes for len, n bytes for values
  uint8_t data[] = {0x00, 0x08, 0x4D, 0x00, 0x0F, 0x00, 0x02, 0x00, 0x0B, 0x0A, 0X8F, 0XC4, 0X42, 0XDE};

  TEST_CMD(data, sizeof(data), range_err_resp);
}

void test_M_len_out_of_range(void) {
  // 1 byte for page num, 2 bytes for offset, 2 bytes for len, n bytes for values
  uint8_t data[] = {0x00, 0x08, 0x4D, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0B, 0x0A, 0XD5, 0X59, 0X52, 0XB7};

  TEST_CMD(data, sizeof(data), range_err_resp);
}


void setup() {
  UNITY_BEGIN();

  RUN_TEST(test_f);
  RUN_TEST(test_unknown_command);
  RUN_TEST(test_M_page_out_of_range);
  RUN_TEST(test_M_offset_out_of_range);
  RUN_TEST(test_M_len_out_of_range);

  UNITY_END();
}

void loop() { }

