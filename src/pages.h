#ifndef PAGES_H
#define PAGES_H

#include <Arduino.h>
#include "flash.h"

struct Page {
  void* pointer;
  size_t len;
};

class Pages {
	public:
    Pages() = default;
    Pages(Page* pages, uint16_t num_pages);

    void init();

    virtual uint8_t* getPageValue(uint16_t pageNum, uint16_t offset);
	  virtual uint32_t getPageCRC(uint16_t pageNum);
	  virtual size_t getPageLen(uint16_t pageNum);

    virtual void loadStoredPages(void);
    virtual bool storePage(uint16_t pageNum);

    virtual bool isIndexOutOfRange(uint16_t pageNum);

    virtual Page* getPages(void);
    virtual Page* getPage(uint16_t pageNum);
    virtual uint16_t getPageNum(void);

    virtual void setPages(Page* pages, uint16_t num_pages);
  private:
    Page* _pages;
    uint16_t _num_pages;
    Flash flash;
};

#endif