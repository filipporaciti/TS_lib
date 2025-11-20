#include "pages.h"

Pages::Pages(const Page* pages) {
	_pages = pages;
}

void* Pages::getPageValue(uint16_t pageNum, uint16_t offset){
	if (pageNum >= (sizeof(_pages)/sizeof(_pages[0]))) {
		return nullptr;
	}
	return (uint8_t*)&_pages[0].pointer + offset;
}

uint32_t Pages::getPageCRC(const void *page, const size_t pageLen){
	uint32_t crc;
 	CRC32 crcCalc;
 	crcCalc.update(page, pageLen);
 	crc = crcCalc.finalize();

 	return crc;
}

size_t Pages::getPageLen(uint16_t pageNum){

}