#ifndef PAGES_MOCK_H
#define PAGES_MOCK_H

#include <Arduino.h>
#include "pages.h"

class PagesMock: public Pages {
  private:
    uint8_t page0[10] = {};
  public:
    PagesMock() {
      reset();
    }

    void reset(void) {
      for (int i = 0; i < 10; i++) {
        page0[i] = i;
      }
    }
    
    uint8_t* getPageValue(uint16_t pageNum, uint16_t offset) override {
      if (isIndexOutOfRange(pageNum) || offset >= getPageLen(pageNum)) {
        return nullptr;
      }
      return page0 + offset;
    }

    uint32_t getPageCRC(uint16_t pageNum) override {
      return 0xFFFFFFFF;
    }

    bool isIndexOutOfRange(uint16_t pageNum) override {
      if (pageNum == 0) {
        return false;
      }
      return true;
    }

    size_t getPageLen(uint16_t pageNum) override { return 10; }

    void loadStoredPages(void) override {}

    bool storePage(uint16_t pageNum) override { return !isIndexOutOfRange(pageNum); }

    Page* getPages(void) override {
      return new Page[1]{{(void*)page0, 10}};
    }

    Page* getPage(uint16_t pageNum) override {
      if (isIndexOutOfRange(pageNum)) {
        return nullptr;
      }
      return new Page{(void*)page0, 10};
    }

    uint16_t getPageNum(void) override { return 1; }

    void setPages(Page* pages, uint16_t num_pages) override {}
};

#endif