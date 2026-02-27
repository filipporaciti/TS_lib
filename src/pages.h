#ifndef PAGES_H
#define PAGES_H

#include <Arduino.h>

struct Page {
  void* pointer;
  size_t len;
};

class Pages {
	public:
    Pages() = default;
    Pages(Page* pages, uint16_t num_pages);

    uint8_t* getPageValue(uint16_t pageNum, uint16_t offset);
	  uint32_t getPageCRC(void *page, size_t pageLen);
	  size_t getPageLen(uint16_t pageNum);

    void loadStoredPages(void);
    bool storePage(uint16_t pageNum);

    bool isIndexOutOfRange(uint16_t pageNum);

    Page* getPages(void);
    Page* getPage(uint16_t pageNum);
    uint16_t getPageNum(void);

    void setPages(Page* pages, uint16_t num_pages);
  private:
    Page* _pages;
    uint16_t _num_pages;
};

#endif