/*
 * TimeSync.h
 *
 *  Created on: 01 lug 2018
 *      Author: maurizio
 */

#ifndef TIMESYNC_H_
#define TIMESYNC_H_

#include <Arduino.h>
#include <SPI.h>
#include <RTClib.h>

class TimeSync {
public:
	static TimeSync* getInstance();
	uint32_t getTimestamp();
	String getDateAsString();
	void syncTime(unsigned long timestamp);
private:
	static TimeSync* instance;
	RTC_DS1307 rtc;
	TimeSync();
};

#endif /* TIMESYNC_H_ */
