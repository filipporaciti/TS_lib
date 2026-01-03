#ifndef RTDATA_H
#define RTDATA_H


#include <Arduino.h>

struct Rt_values {
  void* pointer;
  size_t len;
};

class Rt_data {
	
	public:
		Rt_data() = default;
		Rt_data(Rt_values* rt_values);
		void* getRtData(void);
		size_t getRtDataLen(void);

		Rt_values* getRtValues(void);

		void setRtValues(Rt_values* rt_values);

	private:
		Rt_values* _rt_values;

};


#endif