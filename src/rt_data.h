#ifndef RTDATA_H
#define RTDATA_H


#include <Arduino.h>

struct Rt_values {
  const void* pointer;
  const size_t len;
};

class Rt_data {
	
	public:
		Rt_data() = default;
		Rt_data(const Rt_values* rt_values);
		void* getRtData(void);
		size_t getRtDataLen(void);

		Rt_values* getRtValues(void);

	private:
		const Rt_values* _rt_values;

};


#endif