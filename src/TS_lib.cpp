#include "TS_lib.h"

#include "Arduino.h"

TS_lib::TS_lib(const Stream* s1) {
	_serial1 = s1;
}

void TS_lib::update() {

}
void TS_lib::setRealTimeStruct(const void* rt_data, const size_t structLen) {

}
void TS_lib::setPages(const Page* pages) {

}
void TS_lib::setCodeVersion(const char* code_version){

}