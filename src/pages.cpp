#include "pages.h"
#include <EEPROM.h>
#include <CRC32.h>

Pages::Pages(const Page* pages) {
	_pages = pages;
}

void* Pages::getPageValue(const uint16_t pageNum, const uint16_t offset){
	if (pageNum > (sizeof(*_pages)/sizeof(Page))) {
		return nullptr;
	}
	return ((uint8_t*)_pages[pageNum-1].pointer + offset);
}

uint32_t Pages::getPageCRC(const void *page, const size_t pageLen){
	uint32_t crc;
 	CRC32 crcCalc;
 	crcCalc.update(page, pageLen);
 	crc = crcCalc.finalize();

 	return crc;
}

size_t Pages::getPageLen(const uint16_t pageNum){
	if (pageNum > (sizeof(*_pages)/sizeof(Page))) {
		return 0;
	}
	return _pages[pageNum-1].len;
}

bool Pages::storePage(uint16_t pageNum) {
	if (pageNum > (sizeof(*_pages)/sizeof(Page))) {
		return false;
	}
	noInterrupts();
	for (size_t i = 0; i < _pages[pageNum-1].len; i++) {
	    EEPROM.write(sizeof(Page)*(pageNum-1) + i, ((uint8_t*)_pages[pageNum-1].pointer)[i]);
	}
	interrupts();
	return true;
}


bool Pages::loadStoredPages(void) {
	noInterrupts();
	for (size_t i = 0; i < _pages[0].len; i++) {
	    ((uint8_t*)_pages[0].pointer)[i] = EEPROM.read(0+i);
	}
	interrupts();
}

