#include <Arduino.h>
#include <unity.h>
#include <mock_stream.cpp>
#include "communication_legacy.h"
#include "TS_lib.h"

MockStream mockStream;
Communication_legacy comm(&mockStream);

void setUp(void) {
  mockStream.clear();
  comm = Communication_legacy(&mockStream);
}

void tearDown(void) {}


void check_codeVersion(char* code_version) {
  mockStream.pushByte('Q');
  comm.serialReceiveLegacy();

  TEST_ASSERT_EQUAL_STRING(code_version, mockStream.getTxBuffer().c_str());
}

void test_code_version_command(void) {
  check_codeVersion(DEFAULT_CODE_VERSION);
}

void test_code_version2_command(void) {
  mockStream.pushByte('S');
  comm.serialReceiveLegacy();

  TEST_ASSERT_EQUAL_STRING(DEFAULT_CODE_VERSION, mockStream.getTxBuffer().c_str());
}

void test_protocol_version_command(void) {
  mockStream.pushByte('F');
  comm.serialReceiveLegacy();

  TEST_ASSERT_EQUAL_STRING(DEFAULT_PROTOCOL_VERSION, mockStream.getTxBuffer().c_str());
}

void test_two_param_constructor(void) {
  char custom_code_version[] = "sdsa67678ASHGFAds";
  comm = Communication_legacy(&mockStream, custom_code_version);

  check_codeVersion(custom_code_version);
}

void test_is_legacy(void) {
  TEST_ASSERT_TRUE(Communication_legacy::isLegacy('Q'));
  TEST_ASSERT_TRUE(Communication_legacy::isLegacy('S'));
  TEST_ASSERT_TRUE(Communication_legacy::isLegacy('F'));
  TEST_ASSERT_FALSE(Communication_legacy::isLegacy('X'));
}

void test_serialReceiveLegacy_no_data(void) {
  comm.serialReceiveLegacy();
  TEST_ASSERT_TRUE(mockStream.getTxBuffer().equals(""));
}

void test_set_code_version(void) {
  char new_code_version[] = "asd89SDA())=/d";
  comm.setCodeVersion(new_code_version);

  check_codeVersion(new_code_version);
}

void test_codeVersion_escaping(void) {
  char new_code_version[] = "HIIII pippo";
  comm.setCodeVersion(new_code_version);

  new_code_version[4] = 'f';
  check_codeVersion((char*)"HIIII pippo");
}

void test_codeVersion_limit(void) {
  char long_code_version[] = "1234567890123456789012345678901234567890123456789012345678901234";
  comm.setCodeVersion(long_code_version);

  check_codeVersion((char*)"123456789012345678901234567890123456789012345678901234567890123");
  TEST_ASSERT_EQUAL(63, mockStream.getTxBuffer().length());
}

void test_set_code_version_nullptr(void) {
  comm.setCodeVersion(nullptr);
  check_codeVersion((char*)"");
}

void setup() {

  UNITY_BEGIN();
  
  RUN_TEST(test_code_version_command);
  RUN_TEST(test_code_version2_command);
  RUN_TEST(test_protocol_version_command);
  RUN_TEST(test_two_param_constructor);
  RUN_TEST(test_is_legacy);
  RUN_TEST(test_serialReceiveLegacy_no_data);
  RUN_TEST(test_set_code_version);
  RUN_TEST(test_codeVersion_escaping);
  RUN_TEST(test_codeVersion_limit);
  RUN_TEST(test_set_code_version_nullptr);

  UNITY_END();
}

void loop() { }

