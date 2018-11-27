/*
 * TimeSync.cpp
 *
 *  Created on: 01 lug 2018
 *      Author: maurizio
 */

#include "TimeSync.h"

/* Null, because instance will be initialized on demand. */
TimeSync* TimeSync::instance = 0;

TimeSync* TimeSync::getInstance(){
	if (instance == 0){
		instance = new TimeSync();
	}

	return instance;
}

void TimeSync::begin(){
	Serial.println("Begin RTC");
}

uint32_t TimeSync::getTimestamp() {
	DateTime now = rtc.now();
	return now.unixtime();
}

String TimeSync::getDateAsString() {
	DateTime now = rtc.now();
	String name = String(now.year());

	if(now.month() < 9){
		name += "0";
	}

	name += now.month();

	if(now.day() < 9){
		name += "0";
	}

	name += String(now.day());

	return name;
}

void TimeSync::syncTime(unsigned long timestamp) {
	rtc.adjust(DateTime(timestamp));
}

TimeSync::TimeSync() {
	if (!rtc.begin()) {
		Serial.println("Couldn't find RTC");
		while (1);
	}

	if (!rtc.isrunning()) {
		Serial.println("RTC is NOT running!");

	}
}
