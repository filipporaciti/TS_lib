#include <Arduino.h>
#include <unity.h>
#include "pages.h"
#include "struct.h"

struct page1 p1;
struct page2 p2;
struct Page page_1 = {&p1, sizeof(p1)};
struct Page page_2 = {&p2, sizeof(p2)};

Page pages[] = {page_1, page_2};
Pages myPages(pages, 2);

void setUp(void) {}

void tearDown(void) {}


void test_pages_num(void) {
  TEST_ASSERT_EQUAL(2, myPages.getPageNum());
}

void test_get_page_value(void) {
  p1.x1 = 10;
  p1.x2 = 20;
  p2.x3 = 30;
  p2.x4 = 40;

  uint8_t* x1 = myPages.getPageValue(0, 0);
  uint8_t* x2 = myPages.getPageValue(0, sizeof(p1.x1));
  uint8_t* x3 = myPages.getPageValue(1, 0);
  uint16_t* x4 = (uint16_t*)myPages.getPageValue(1, sizeof(p2.x3));

  TEST_ASSERT_EQUAL(10, *x1);
  TEST_ASSERT_EQUAL(20, *x2);
  TEST_ASSERT_EQUAL(30, *x3);
  TEST_ASSERT_EQUAL(40, *x4);
}


void setup() {
  UNITY_BEGIN();
  
  RUN_TEST(test_pages_num);
  RUN_TEST(test_get_page_value);

  UNITY_END();
}

void loop() { }

