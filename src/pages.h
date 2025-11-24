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

    void* getPageValue(const uint16_t pageNum, const uint16_t offset);
	  uint32_t getPageCRC(const void *page, const size_t pageLen);
	  size_t getPageLen(const uint16_t pageNum);

    bool loadStoredPages(void);
    bool storePage(uint16_t pageNum);

    bool isIndexOutOfRange(uint16_t pageNum);

  private:
    const Page* _pages;
};

#endif