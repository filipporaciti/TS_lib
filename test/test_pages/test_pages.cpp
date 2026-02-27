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


void setup() {
  UNITY_BEGIN();
  
  RUN_TEST(test_pages_num);

  UNITY_END();
}

void loop() { }

