#include "TS_lib.h"
#include <Arduino.h>
#include "communication.h"

#include <EEPROM.h>


char DEFAULT_CODE_VERSION[] = "TSlib_11-2025";

TS_lib::TS_lib(Stream* s1) 
	: TS_lib(s1, nullptr, nullptr, 0) {} 

TS_lib::TS_lib(Stream* s1, Rt_values* rt_values, Page* pages, uint16_t num_pages) {
	setCodeVersion(DEFAULT_CODE_VERSION);
	_serial1 = s1;
	_rt_data = Rt_data(rt_values);
	_pages = Pages(pages, num_pages);
	_comm = Communication(_serial1, DEFAULT_CODE_VERSION, &_rt_data, &_pages);
	_comm_legacy = Communication_legacy(_serial1, DEFAULT_CODE_VERSION);
}

void TS_lib::init() {
	_pages.init();
}

void TS_lib::update() {
	if (_serial1->available() > 0) {
		if (_comm.isReady() && _comm_legacy.isLegacy(_serial1->peek())) {
			_comm_legacy.serialReceiveLegacy();
		} else {
			_comm.serialReceive();
		}
	}
}


void TS_lib::setCodeVersion(char* code_version) {
	if (code_version == nullptr) code_version = (char*)"";
	
	strncpy(_code_version, code_version, sizeof(_code_version) - 1);
	_code_version[sizeof(_code_version) - 1] = '\0';

	_comm.setCodeVersion(code_version);
	_comm_legacy.setCodeVersion(code_version);
}

char* TS_lib::getCodeVersion(void) {
	char* code_version_copy = new char[sizeof(_code_version)];
	strncpy(code_version_copy, _code_version, sizeof(_code_version));
	code_version_copy[sizeof(_code_version) - 1] = '\0';
	return code_version_copy;
}

void TS_lib::setPages(Page* pages, uint16_t num_pages) {
	_pages.setPages(pages, num_pages);
	_pages.loadStoredPages();
}

void TS_lib::setRtData(Rt_values* rt_values) {
	_rt_data.setRtValues(rt_values);
}

Page* TS_lib::getPages(void) {
	return _pages.getPages();
}

Page* TS_lib::getPage(uint16_t pageNum) {
	return _pages.getPage(pageNum);
}

uint16_t TS_lib::getPageNum(void) {
	return _pages.getPageNum();
}

Rt_values* TS_lib::getRtData(void) {
	return _rt_data.getRtValues();
}
