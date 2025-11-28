#include <TS_lib.h>
#include "structs.h"

// create TS_lib object
TS_lib ts = TS_lib(&Serial);

// create structs
realtime_data rt_data;
page1 p1;

void setup() {
	Serial.begin(115200);
	while (Serial.available());

	ts.setCodeVersion("MyCodeVersion_11-2025");

	// WARNING!!! x2 only exists in this scope (setup function); so when loop function will be execute, x2 will no longer exist => crash (same for pages)
	// Rt_values x2 = Rt_values{&rt_data, sizeof(rt_data)};
	Rt_values* rt_values = new Rt_values{&rt_data, sizeof(rt_data)};
	ts.setRtData(rt_values);

	Page page_1 = {&p1, sizeof(p1)};
	Page* pages = new Page[1]{page_1};
	ts.setPages(pages, 1);

	ts.getPages();
	ts.getPage(0);
	ts.getRtData();
	ts.getCodeVersion();
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

	times++;
	if ((millis()-prev_time) >= 1000) {
		prev_time = millis();
		rt_data.cyclesPerSecond = times;
		times = 0;
	}
}
