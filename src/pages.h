

void* getPageValue(uint16_t pageNum, uint16_t offset);

uint32_t getPageCRC(const void *page, size_t pageLen);

size_t getPageLen(uint16_t pageNum);