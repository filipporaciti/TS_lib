#ifndef RT_DATA_MOCK_H
#define RT_DATA_MOCK_H

#include <Arduino.h>
#include "rt_data.h"

class RtDataMock: public Rt_data {
  public:
    RtDataMock() = default;
    
    void* getRtData(void) override {
      return (void*)"\x01\x02\x03\x04";
    }

		size_t getRtDataLen(void) override {
      return 4;
    }

		Rt_values* getRtValues(void) override {

      return new Rt_values{(void*)"\x01\x02\x03\x04", 4};
    }

		void setRtValues(Rt_values* rt_values) override { }
};

#endif