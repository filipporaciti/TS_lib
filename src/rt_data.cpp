#include <Arduino.h>
#include "rt_data.h"

Rt_data::Rt_data(Rt_values* rt_values) {
  _rt_values = rt_values;
}


void* Rt_data::getRtData() {
	if (_rt_values == nullptr) return nullptr;
	return _rt_values->pointer;
}
size_t Rt_data::getRtDataLen() {
	if (_rt_values == nullptr) return 0;
	return _rt_values->len;
}

Rt_values* Rt_data::getRtValues(void) {
	return _rt_values;
}

void Rt_data::setRtValues(Rt_values* rt_values) {
	_rt_values = rt_values;
}