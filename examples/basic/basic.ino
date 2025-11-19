#include <TS_lib.h>
#include "structs.h"

struct page1 p1;
struct Page page_1 = {&p1, sizeof(p1)};
Page pages[2] = {page_1};

TS_lib ts = TS_lib(&Serial, pages);
struct realtime_data rt_data;


void setup() {
	Serial.begin(115200);

	while (Serial.available())

	ts.setRealTimeStruct(&rt_data, sizeof(rt_data));
}

void loop() {
	ts.update();
	changeValue();
}

unsigned long prev_time;
unsigned long times;
void changeValue() {
	rt_data.seconds = millis();
	rt_data.tps = (millis()/10)%100;
	rt_data.rpm = (millis())%p1.rpmMaxLimit;

	times++;
	if ((millis()-prev_time) >= 1000) {
		prev_time = millis();
		rt_data.cyclesPerSecond = times;
		times = 0;
	}
}