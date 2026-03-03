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
		virtual void* getRtData(void);
		virtual size_t getRtDataLen(void);

		virtual Rt_values* getRtValues(void);

		virtual void setRtValues(Rt_values* rt_values);

	private:
		Rt_values* _rt_values;

};


#endif