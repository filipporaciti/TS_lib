

struct page1 {
  uint8_t x1;
  uint8_t x2;
};

struct page2 {
  uint8_t x3;
  uint8_t x4;
  uint16_t x5;  // esp32 wants 16 bit values to be aligned
};