#include "TS_lib.h"
#include "Arduino.h"
#include "communication.h"

TS_lib::TS_lib(const Stream* s1) 
	: TS_lib(s1, &Rt_values{}, &Page{}) {} 

TS_lib::TS_lib(const Stream* s1, const Rt_values* rt_data, const Page* pages) {
	_serial1 = s1;
	_rt_data = Rt_data(rt_data);
	_pages = Pages(pages);
	_pages.loadStoredPages();
	_comm = Communication(_serial1, &_rt_data, &_pages);
	_comm_legacy = Communication_legacy(_serial1);
}

void TS_lib::update() {
	if (_serial1->available() > 0) {
		if (_comm_legacy.isLegacy(_serial1->peek())) {
			_comm_legacy.serialReceiveLegacy();
		} else {
			_comm.serialReceive();
		}
	}
}


void TS_lib::setCodeVersion(const char* code_version) {
	CODE_VERSION = code_version;
}

char* TS_lib::getCodeVersion(void) {
	return CODE_VERSION;
}

void TS_lib::setPages(const Page* pages) {
	_pages = Pages(pages);
}

void TS_lib::setRtData(const Rt_values* rt_data) {
	_rt_data = Rt_data(rt_data);
}

Page* TS_lib::getPages(void) {
	return _pages.getPages();
}

Page* TS_lib::getPage(const uint16_t pageNum) {
	_pages.getPage(pageNum);
}

Rt_values* TS_lib::getRtData(void) {
	return _rt_data.getRtValues();
}
