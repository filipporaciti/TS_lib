#ifndef PAGES_MOCK_H
#define PAGES_MOCK_H

#include <Arduino.h>
#include "pages.h"

class PagesMock: public Pages {
  private:
  public:
    PagesMock() = default;
    
    uint8_t* getPageValue(uint16_t pageNum, uint16_t offset) override {
      if (isIndexOutOfRange(pageNum) || offset >= getPageLen(pageNum)) {
        return nullptr;
      }
      return (uint8_t*)"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09" + offset;
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
      return new Page[1]{{(void*)"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09", 10}};
    }

    Page* getPage(uint16_t pageNum) override {
      if (isIndexOutOfRange(pageNum)) {
        return nullptr;
      }
      return new Page{(void*)"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09", 10};
    }

    uint16_t getPageNum(void) override { return 1; }

    void setPages(Page* pages, uint16_t num_pages) override {}
};

#endif