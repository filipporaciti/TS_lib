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
  TEST_ASSERT_EQUAL_MEMORY(range_err_resp, mockStream.getTxBuffer(), 7);
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

  TEST_CMD(data, sizeof(data), range_err_resp);
}

void test_p_offset_out_of_range(void) {
  uint8_t data[] = {0x00, 0x06, 0x70, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0XC3, 0X66, 0XAF, 0X3F};

  TEST_CMD(data, sizeof(data), range_err_resp);
}

void test_p_len_out_of_range(void) {
  uint8_t data[] = {0x00, 0x06, 0x70, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0X04, 0XC6, 0XBE, 0X9D};

  TEST_CMD(data, sizeof(data), range_err_resp);
}

void test_p_more_data(void) {
  uint8_t data[] = {0x00, 0x06, 0x70, 0x00, 0x02, 0x00, 0x03, 0x00, 0X3B, 0XC4, 0X37, 0X2A};
  uint8_t response[] = {0x00, 0x04, 0x00, 0x02, 0x03, 0x04, 0X0E, 0X80, 0X9C, 0XA8};

  TEST_CMD(data, sizeof(data), response);
}

void test_d(void) {
  // 1 byte for page num
  uint8_t data[] = {0x00, 0x02, 0x64, 0x00, 0X40, 0X48, 0XBC, 0X5C};
  uint8_t response[] = {0x00, 0x05, 0x00, 0XFF, 0XFF, 0XFF, 0XFF, 0X18, 0X99, 0XD7, 0XFE};

  TEST_CMD(data, sizeof(data), response);
}

void test_A(void) {
  uint8_t data[] = {0x00, 0x01, 0x41, 0XD3, 0XD9, 0X9E, 0X8B};
  uint8_t response[] = {0x00, 0x05, 0x00, 0x01, 0x02, 0x03, 0x04, 0X51, 0X5A, 0XD3, 0XCC};

  TEST_CMD(data, sizeof(data), response);
}

void test_b(void) {
  // 1 byte for page num
  uint8_t data[] = {0x00, 0x02, 0x62, 0x00, 0X16, 0X12, 0X1B, 0XDA};
  uint8_t response[] = {0x00, 0x01, 0x04, 0XD5, 0X6F, 0X2B, 0X94};

  TEST_CMD(data, sizeof(data), response);
}

void test_b_page_out_of_range(void) {
  // 1 byte for page num
  uint8_t data[] = {0x00, 0x02, 0x62, 0x01, 0X61, 0X15, 0X2B, 0X4C};

  mockStream.pushBytes(data, sizeof(data));
  comm.serialReceive();

  TEST_ASSERT_EQUAL(0, mockStream.getTxBufferLen());
  TEST_ASSERT_TRUE(comm.isReady());
}

void test_M(void) {
  // 1 byte for page num, 2 bytes for offset, 2 bytes for len, n bytes for values
  uint8_t data[] = {0x00, 0x08, 0x4D, 0x00, 0x00, 0x00, 0x02, 0x00, 0x0B, 0x0A, 0X7E, 0X92, 0XF0, 0X0B};
  uint8_t response[] = {0x00, 0x01, 0x00, 0XD2, 0X02, 0XEF, 0X8D};

  TEST_CMD(data, sizeof(data), response);

  TEST_ASSERT_EQUAL(0x0A, pagesMock.getPageValue(0, 0)[0]);
  TEST_ASSERT_EQUAL(0x0B, pagesMock.getPageValue(0, 1)[0]);
}

void test_I(void) {
  uint8_t data[] = {0x00, 0x01, 0x49, 0XDD, 0X02, 0X16, 0XB9};
  uint8_t response[] = {0x00, 0x02, 0x00, 0x01, 0X36, 0XDE, 0X22, 0X69};

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
  RUN_TEST(test_p_len_out_of_range);
  RUN_TEST(test_p_more_data);
  RUN_TEST(test_d);
  RUN_TEST(test_A);
  RUN_TEST(test_b);
  RUN_TEST(test_b_page_out_of_range);
  RUN_TEST(test_M);
  RUN_TEST(test_I);

  UNITY_END();
}

void loop() { }

