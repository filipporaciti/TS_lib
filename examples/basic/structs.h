#include <Arduino.h>

struct realtime_data {
  uint32_t  seconds;
  uint32_t  cyclesPerSecond;
  uint16_t  rpm;
  uint8_t   tps;
  uint8_t   synccnt;
};

// **************************

struct page1 {
  uint8_t crankTeeth;
  uint8_t crankMissingTeeth;

  uint8_t tpsMin;
  uint8_t tpsMax;

  uint16_t rpmMaxLimit;
};

struct page2 {
    uint8_t fuelVeTable[16][16];
};