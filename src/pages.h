#ifndef PAGES_H
#define PAGES_H

#include <Arduino.h>

struct Page {
  const void* pointer;
  const size_t len;
};

class Pages {
	public:
    Pages() = default;
    Pages(const Page* pages);

    void* getPageValue(uint16_t pageNum, uint16_t offset);
	uint32_t getPageCRC(const void *page, size_t pageLen);
	size_t getPageLen(uint16_t pageNum);
	uint32_t getPageCRC(const void *page, const size_t pageLen);

  private:
    const Page* _pages;
};

#endif