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

void setUp(void) {
  p1.x1 = 0;
  p1.x2 = 0;
  p2.x3 = 0;
  p2.x4 = 0;
}

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

void test_get_page_value_out_of_range(void) {
  uint8_t* value = myPages.getPageValue(2, 0);
  TEST_ASSERT_NULL(value);

  value = myPages.getPageValue(-1, 0);
  TEST_ASSERT_NULL(value);
}

void test_get_page_value_offset_out_of_range(void) {
  uint8_t* value = myPages.getPageValue(0, sizeof(p1));
  TEST_ASSERT_NULL(value);
}

void test_get_page_crc(void) {
  uint32_t crc1 = myPages.getPageCRC(0);
  uint32_t crc2 = myPages.getPageCRC(1);

  TEST_ASSERT_EQUAL(1104745215, crc1);
  TEST_ASSERT_EQUAL(4282505490, crc2);

  p1.x1 = 1;
  p1.x2 = 2;
  p2.x3 = 1;
  p2.x4 = 2;
  crc1 = myPages.getPageCRC(0);
  crc2 = myPages.getPageCRC(1);

  TEST_ASSERT_EQUAL(3066839698, crc1);
  TEST_ASSERT_EQUAL(3434467751, crc2);
}

void test_get_page_crc_out_of_range(void) {
  uint32_t crc = myPages.getPageCRC(2);
  TEST_ASSERT_EQUAL(0, crc);

  crc = myPages.getPageCRC(-1);
  TEST_ASSERT_EQUAL(0, crc);
}

void test_get_page_len(void) {
  size_t len1 = myPages.getPageLen(0);
  size_t len2 = myPages.getPageLen(1);

  TEST_ASSERT_EQUAL(sizeof(p1), len1);
  TEST_ASSERT_EQUAL(sizeof(p2), len2);
}

void test_get_page_len_out_of_range(void) {
  size_t len = myPages.getPageLen(2);
  TEST_ASSERT_EQUAL(0, len);

  len = myPages.getPageLen(-1);
  TEST_ASSERT_EQUAL(0, len);
}

void test_index_out_of_range(void) {
  TEST_ASSERT_TRUE(myPages.isIndexOutOfRange(2));
  TEST_ASSERT_TRUE(myPages.isIndexOutOfRange(-1));
  TEST_ASSERT_FALSE(myPages.isIndexOutOfRange(0));
  TEST_ASSERT_FALSE(myPages.isIndexOutOfRange(1));
}

void test_store_page(void) {
  myPages.storePage(0);
  myPages.storePage(1);

  p1.x1 = 1;
  p1.x2 = 2;
  p2.x3 = 3;
  p2.x4 = 4;

  bool result1 = myPages.storePage(0);

  TEST_ASSERT_TRUE(result1);

  p1.x1 = 5;
  p1.x2 = 6;
  p2.x3 = 7;
  p2.x4 = 8;

  myPages.loadStoredPages();

  TEST_ASSERT_EQUAL(1, p1.x1);
  TEST_ASSERT_EQUAL(2, p1.x2);
  TEST_ASSERT_EQUAL(0, p2.x3);
  TEST_ASSERT_EQUAL(0, p2.x4);
}


void setup() {
  UNITY_BEGIN();
  
  RUN_TEST(test_pages_num);
  RUN_TEST(test_get_page_value);
  RUN_TEST(test_get_page_value_out_of_range);
  RUN_TEST(test_get_page_value_offset_out_of_range);

  RUN_TEST(test_get_page_crc);
  RUN_TEST(test_get_page_crc_out_of_range);

  RUN_TEST(test_get_page_len);
  RUN_TEST(test_get_page_len_out_of_range);

  RUN_TEST(test_index_out_of_range);

  RUN_TEST(test_store_page);

  UNITY_END();
}

void loop() { }

