#include "TS_lib.h"
#include "Arduino.h"
#include "communication.h"

TS_lib::TS_lib(const Stream* s1) {
	_serial1 = s1;
	_comm = Communication(_serial1);
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
void TS_lib::setRealTimeStruct(const void* rt_data, const size_t structLen) {

}
void TS_lib::setPages(const Page* pages) {

}
void TS_lib::setCodeVersion(const char* code_version){

}