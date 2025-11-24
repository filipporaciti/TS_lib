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