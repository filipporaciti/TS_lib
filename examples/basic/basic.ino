#include <TS_lib.h>
#include "structs.h"

// create page object
struct page1 p1;
struct Page page_1 = {&p1, sizeof(p1)};
Page pages[] = {page_1};

// create realtime object
struct realtime_data rt_data;
struct Rt_values rt_values = {&rt_data, sizeof(rt_data)};

// create TS_lib object
TS_lib ts = TS_lib(&Serial, &rt_values, pages, 1);


void setup() {
	Serial.begin(115200);
	while (Serial.available());

	ts.init();
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

	if (p1.rpmMaxLimit == 0) {
		rt_data.rpm;
	} else {
		rt_data.rpm = (millis())%p1.rpmMaxLimit;
	}

	times++;
	if ((millis()-prev_time) >= 1000) {
		prev_time = millis();
		rt_data.cyclesPerSecond = times;
		times = 0;
	}
}
