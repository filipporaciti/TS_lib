#include <Arduino.h>
#include <unity.h>
#include <mock_stream.cpp>
#include "TS_lib.h"

MockStream mockStream;
TS_lib ts(&mockStream);

void setUp(void) {
  mockStream.clear();
  ts = TS_lib(&mockStream);
}

void tearDown(void) {}


void test_update_legacy(void) {
  mockStream.pushByte('Q');
  TEST_ASSERT_EQUAL(0, mockStream.getTxBufferLen());
  ts.update();
  TEST_ASSERT_EQUAL_STRING(DEFAULT_CODE_VERSION, mockStream.getTxBuffer());
}

void test_update(void) {
  uint8_t data[] = {0x00, 0x01, 0x51, 0xCE, 0x6E, 0x8E, 0xEF};
  uint8_t expected_len = 2 + 1 + 13 + 4;

  mockStream.pushBytes(data, sizeof(data));

  TEST_ASSERT_EQUAL(0, mockStream.getTxBufferLen());
  ts.update();
  TEST_ASSERT_EQUAL(expected_len, mockStream.getTxBufferLen());
}

void test_update_not_ready(void) {
  uint8_t data[] = {0x00, 0x01, 0x51, 0xCE, 0x6E, 0x8E, 0xEF};
  uint8_t expected_len = 2 + 1 + 13 + 4;

  mockStream.pushBytes(data, 2);
  ts.update();

  TEST_ASSERT_EQUAL(0, mockStream.getTxBufferLen());

  mockStream.pushBytes(data+2, 1);
  ts.update();

  TEST_ASSERT_EQUAL(0, mockStream.getTxBufferLen());

  mockStream.pushBytes(data+3, 4);
  ts.update();
  
  TEST_ASSERT_EQUAL(expected_len, mockStream.getTxBufferLen());
}

void test_set_pages(void) {

}


void setup() {
  UNITY_BEGIN();

  RUN_TEST(test_update_legacy);
  RUN_TEST(test_update);
  RUN_TEST(test_update_not_ready);

  UNITY_END();
}

void loop() { }

