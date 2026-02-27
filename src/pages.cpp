#include "pages.h"
#include <EEPROM.h>
#include <CRC32.h>

Pages::Pages(Page* pages, uint16_t num_pages) {
	_pages = pages;
	_num_pages = num_pages;
}

uint8_t* Pages::getPageValue(uint16_t pageNum, uint16_t offset){
	if (isIndexOutOfRange(pageNum) || offset >= _pages[pageNum].len) {
		return nullptr;
	}
	return ((uint8_t*)_pages[pageNum].pointer + offset);
}

uint32_t Pages::getPageCRC(uint16_t pageNum){
	if (_pages == nullptr) return 0;

	void* page = getPageValue(pageNum, 0);
	size_t pageLen = getPageLen(pageNum);

	uint32_t crc;
 	CRC32 crcCalc;
 	crcCalc.update(page, pageLen);
 	crc = crcCalc.finalize();

 	return crc;
}

size_t Pages::getPageLen(uint16_t pageNum){
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
	if (_pages == nullptr) return true;
	if (_num_pages == 0) return true;
	if (pageNum >= _num_pages) {
		return true;
	}
	return false;
}


Page* Pages::getPages(void) {
	return _pages;
}

Page* Pages::getPage(uint16_t pageNum) {
	if (isIndexOutOfRange(pageNum)) {
		return nullptr;
	}
	return &_pages[pageNum];
}

uint16_t Pages::getPageNum(void) {
	return _num_pages;
}

void Pages::setPages(Page* pages, uint16_t num_pages) {
	_pages = pages;
	_num_pages = num_pages;
}

