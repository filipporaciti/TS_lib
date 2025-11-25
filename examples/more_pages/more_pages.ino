#include <TS_lib.h>
#include "structs.h"

// create page object
struct page1 p1;
struct page2 p2;
struct Page page_1 = {&p1, sizeof(p1)};
struct Page page_2 = {&p2, sizeof(p2)};
Page pages[] = {page_1, page_2};

// create realtime object
struct realtime_data rt_data;
struct Rt_values rt_values = {&rt_data, sizeof(rt_data)};

// create TS_lib object
TS_lib ts = TS_lib(&Serial, &rt_values, pages);


void setup() {
	Serial.begin(115200);
	while (Serial.available());
}

void loop() {
	ts.update(); // update serial communication
	changeValue();
}

unsigned long prev_time;
unsigned long times;
void changeValue() {
	rt_data.seconds = millis();
	rt_data.tps = (millis()/10)%100;
	rt_data.rpm = (millis())%p1.rpmMaxLimit;

	rt_data.sparkAdvance = calcAdv();

	times++;
	if ((millis()-prev_time) >= 1000) {
		prev_time = millis();
		rt_data.cyclesPerSecond = times;
		times = 0;
	}
}

uint8_t calcAdv() {
	for (uint8_t i=0; i<(10-1); i++) {
		if (rt_data.rpm >= p2.sparkAdvance_rpm[i] && rt_data.rpm <= p2.sparkAdvance_rpm[i+1]) {
			return map(rt_data.rpm, p2.sparkAdvance_rpm[i], p2.sparkAdvance_rpm[i+1], p2.sparkAdvance_deg[i], p2.sparkAdvance_deg[i+1]);
		}
	}
	return 0;
}