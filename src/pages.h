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
    Pages(const Page* pages, const uint16_t num_pages);

    void* getPageValue(const uint16_t pageNum, const uint16_t offset);
	  uint32_t getPageCRC(const void *page, const size_t pageLen);
	  size_t getPageLen(const uint16_t pageNum);

    void loadStoredPages(void);
    bool storePage(uint16_t pageNum);

    bool isIndexOutOfRange(uint16_t pageNum);

    Page* getPages(void);
    Page* getPage(const uint16_t pageNum);

    void setPages(const Page* pages, const uint16_t num_pages);
  private:
    const Page* _pages;
    uint16_t _num_pages;
};

#endif