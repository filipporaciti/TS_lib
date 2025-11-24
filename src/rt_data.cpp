#include <Arduino.h>
#include "rt_data.h"

Rt_data::Rt_data(const Rt_values* rt_values) {
  _rt_values = rt_values;
}


void* Rt_data::getRtData() {
  uint8_t data[12] = {};
	return &data;
}
size_t Rt_data::getRtDataLen() {
	return 12;
}