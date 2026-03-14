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

void test_code_version(void) {
  char* code_version = ts.getCodeVersion();
  TEST_ASSERT_EQUAL_STRING(DEFAULT_CODE_VERSION, code_version);
}

void test_set_code_version(void) {
  char new_code_version[] = "ds798DSAGD%&$/ds";
  ts.setCodeVersion(new_code_version);
  char* code_version = ts.getCodeVersion();
  TEST_ASSERT_EQUAL_STRING(new_code_version, code_version);
}

void test_code_version_ref_escape(void) {
  char new_code_version[] = "ds798DSAGD%&$/ds";
  ts.setCodeVersion(new_code_version);
  new_code_version[4] = 'X';
  char* code_version = ts.getCodeVersion();
  TEST_ASSERT_EQUAL_STRING("ds798DSAGD%&$/ds", code_version);
}

void test_code_version_limit(void) {
  char long_code_version[] = "1234567890123456789012345678901234567890123456789012345678901234";
  ts.setCodeVersion(long_code_version);
  char* code_version = ts.getCodeVersion();
  TEST_ASSERT_EQUAL_STRING("123456789012345678901234567890123456789012345678901234567890123", code_version);
}

void test_code_version_ref_escape2(void) {
  char* code_version = ts.getCodeVersion();
  code_version[4] = 'X';
  TEST_ASSERT_EQUAL_STRING(DEFAULT_CODE_VERSION, ts.getCodeVersion());
}

void test_set_code_version_nullptr(void) {
  ts.setCodeVersion(nullptr);
  char* code_version = ts.getCodeVersion();
  TEST_ASSERT_EQUAL_STRING("", code_version);
}


void setup() {
  UNITY_BEGIN();

  RUN_TEST(test_code_version);
  RUN_TEST(test_set_code_version);
  RUN_TEST(test_code_version_ref_escape);
  RUN_TEST(test_code_version_limit);
  RUN_TEST(test_code_version_ref_escape2);
  RUN_TEST(test_set_code_version_nullptr);

  UNITY_END();
}

void loop() { }

