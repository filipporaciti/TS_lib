#include "TS_lib.h"

#include "Arduino.h"

TS_lib::TS_lib(Stream* s1) {
	_serial1 = s1;
}