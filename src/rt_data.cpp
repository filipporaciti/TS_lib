#include <Arduino.h>
#include "rt_data.h"

Rt_data::Rt_data(const Rt_values* rt_values) {
  _rt_values = rt_values;
}


void* Rt_data::getRtData() {
	return _rt_values->pointer;
}
size_t Rt_data::getRtDataLen() {
	return _rt_values->len;
}

Rt_values* Rt_data::getRtValues(void) {
	return _rt_values;
}

void Rt_data::setRtValues(Rt_values* rt_values) {
	_rt_values = rt_values;
}