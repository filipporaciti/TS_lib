#include <Arduino.h>
#include <unity.h>
#include "rt_data.h"
#include "struct.h"

struct realtime_data rd;
struct Rt_values rt_values = {&rd, sizeof(rd)};
Rt_data rt_data(&rt_values);

void setUp(void) {
  rt_data = Rt_data(&rt_values);
  rd.x1 = 0;
  rd.x2 = 0;
  rd.x3 = 0;
}

void tearDown(void) {}

void test_get_rt_data(void) {
  struct realtime_data* data = (struct realtime_data*)rt_data.getRtData();
  TEST_ASSERT_NOT_NULL(data);
  TEST_ASSERT_EQUAL(0, data->x1);
  TEST_ASSERT_EQUAL(0, data->x2);
  TEST_ASSERT_EQUAL(0, data->x3);
}

void test_get_rt_data_len(void) {
  size_t len = rt_data.getRtDataLen();
  TEST_ASSERT_EQUAL(sizeof(rd), len);
}

void test_get_rt_values(void) {
  Rt_values* values = rt_data.getRtValues();
  TEST_ASSERT_NOT_NULL(values);
  TEST_ASSERT_EQUAL(&rd, values->pointer);
  TEST_ASSERT_EQUAL(sizeof(rd), values->len);
}

void test_set_rt_values(void) {
  struct realtime_data new_rd = {1, 2, 3};
  Rt_values new_rt_values = {&new_rd, sizeof(new_rd)};
  rt_data.setRtValues(&new_rt_values);

  struct realtime_data* data = (struct realtime_data*)rt_data.getRtData();
  TEST_ASSERT_NOT_NULL(data);
  TEST_ASSERT_EQUAL(data, &new_rd);
}

void test_null_constructor(void) {
  rt_data = Rt_data(nullptr);

  void* data = rt_data.getRtData();
  size_t len = rt_data.getRtDataLen();

  TEST_ASSERT_NULL(rt_data.getRtValues());
  TEST_ASSERT_NULL(data);
  TEST_ASSERT_EQUAL(0, len);
}

void test_null_set_rt_values(void) {
  rt_data.setRtValues(nullptr);

  struct realtime_data* data = (struct realtime_data*)rt_data.getRtData();
  size_t len = rt_data.getRtDataLen();

  TEST_ASSERT_NOT_NULL(rt_data.getRtValues());
  TEST_ASSERT_EQUAL(sizeof(rd), len);
  TEST_ASSERT_EQUAL(0, data->x1);
  TEST_ASSERT_EQUAL(0, data->x2);
  TEST_ASSERT_EQUAL(0, data->x3);
}

void test_wanted_reference_escape(void) {
  struct realtime_data* data = (struct realtime_data*)rt_data.getRtData();
  TEST_ASSERT_EQUAL(0, data->x1);
  rd.x1 = 42;
  TEST_ASSERT_EQUAL(42, data->x1);
}


void setup() {
  UNITY_BEGIN();

  RUN_TEST(test_get_rt_data);
  RUN_TEST(test_get_rt_data_len);
  RUN_TEST(test_get_rt_values);
  RUN_TEST(test_set_rt_values);
  RUN_TEST(test_null_constructor);
  RUN_TEST(test_null_set_rt_values);

  RUN_TEST(test_wanted_reference_escape);

  UNITY_END();
}

void loop() { }

