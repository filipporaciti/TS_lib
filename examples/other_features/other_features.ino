#include <TS_lib.h>
#include "structs.h"

// create TS_lib object
TS_lib ts = TS_lib(&Serial);

struct realtime_data rt_data;
struct page1 p1;

void setup() {
	Serial.begin(115200);
	while (Serial.available());


	// create page object

	struct Page page_1 = {&p1, sizeof(p1)};
	Page pages[] = {page_1};

	// create realtime object
	struct Rt_values rt_values = {&rt_data, sizeof(rt_data)};


	ts.setCodeVersion("MyCodeVersion_11-2025");
	ts.setPages(pages);
	ts.setRtData(&rt_values);

	// you can also get values
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
