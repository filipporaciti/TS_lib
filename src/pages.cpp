#include "pages.h"
#include <EEPROM.h>
#include <CRC32.h>

Pages::Pages(const Page* pages) {
	_pages = pages;
}

void* Pages::getPageValue(const uint16_t pageNum, const uint16_t offset){
	if (isIndexOutOfRange(pageNum)) {
		return nullptr;
	}
	return ((uint8_t*)_pages[pageNum].pointer + offset);
}

uint32_t Pages::getPageCRC(const void *page, const size_t pageLen){
	uint32_t crc;
 	CRC32 crcCalc;
 	crcCalc.update(page, pageLen);
 	crc = crcCalc.finalize();

 	return crc;
}

size_t Pages::getPageLen(const uint16_t pageNum){
	if (isIndexOutOfRange(pageNum)) {
		return 0;
	}
	return _pages[pageNum].len;
}

bool Pages::storePage(uint16_t pageNum) {
	if (isIndexOutOfRange(pageNum)) {
		return false;
	}
	noInterrupts();
	uint8_t index = 0;
	uint32_t position = 0;

	while (index < pageNum) {
		position += _pages[index].len;
		index++;
	}

	for (size_t i = 0; i < _pages[pageNum].len; i++) {
	    EEPROM.write(position+i, ((uint8_t*)_pages[pageNum].pointer)[i]);
	}
	interrupts();
	return true;
}


void Pages::loadStoredPages(void) {
	noInterrupts();
	uint8_t index = 0;
	uint32_t position = 0;

	while (!isIndexOutOfRange(index)) {
		for (size_t i = 0; i < _pages[index].len; i++) {
		    ((uint8_t*)_pages[index].pointer)[i] = EEPROM.read(position+i);
		}
		position += _pages[index].len;
		index++;
	}
	interrupts();
}

bool Pages::isIndexOutOfRange(uint16_t pageNum) {
	if (pageNum >= (sizeof(*_pages)/sizeof(Page))) {
		return true;
	}
	return false;
}


Page* Pages::getPages(void) {
	return _pages;
}

Page* Pages::getPage(const uint16_t pageNum) {
	return &_pages[pageNum];
}
