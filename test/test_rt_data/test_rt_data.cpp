#include <Arduino.h>
#include <unity.h>
#include "rt_data.h"
#include "struct.h"

struct realtime_data rd;
struct Rt_values rt_values = {&rd, sizeof(rd)};
Rt_data rt_data(&rt_values);

void setUp(void) {
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


void setup() {
  UNITY_BEGIN();

  RUN_TEST(test_get_rt_data);
  RUN_TEST(test_get_rt_data_len);

  UNITY_END();
}

void loop() { }

