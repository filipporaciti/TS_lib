#include <Arduino.h>

struct realtime_data {
  uint32_t  seconds;
  uint32_t  cyclesPerSecond;
  uint16_t  rpm;
  uint8_t   tps;
  uint8_t   synccnt;
  uint8_t   sparkAdvance;
  uint8_t   padding[3]; // byte alignment for esp32
};

// **************************

struct page1 {
  uint8_t crankTeeth;
  uint8_t crankMissingTeeth;

  uint8_t tpsMin; // analog input value
  uint8_t tpsMax; // analog input value

  uint16_t rpmMaxLimit;
};

struct page2 {
  uint16_t sparkAdvance_rpm[10];
  uint8_t  sparkAdvance_deg[10];
};