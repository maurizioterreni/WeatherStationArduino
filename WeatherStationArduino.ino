#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <RTClib.h>


#include "WeatherSensor.h"
#include "DataLogger.h"
#include "WebClient.h"


String version = "0.3.12";

unsigned long previousMillis_url = millis();
unsigned long previousMillis_update_sensor = millis();
const long interval_url = 10000;//900000; //15min
const long interval_update_sensor = 5000;//60000; //1min

const int ledOKPin = 13;
const int ledKOPin = 13;

RTC_DS1307 rtc;

void setup() {
	Wire.begin();

	Serial.begin(9600);

	Serial.println("Weather Station Arduino v" + version);


	if (! rtc.begin()) {
		Serial.println("Couldn't find RTC");
		while (1);
	}

	if (! rtc.isrunning()) {
		Serial.println("RTC is NOT running!");

	}


	rtc.adjust(DateTime(2018, 6, 29, 14, 34, 0));

	//Serial.println(WebClient::getInstance()->getTimestamp());

}

void loop() {
	unsigned long currentMillis = millis();

	if(currentMillis - previousMillis_update_sensor >= interval_url){
		digitalWrite(ledOKPin, HIGH);
		previousMillis_update_sensor  = currentMillis;
		WeatherSensor::getInstance()->updateSensor();

		Serial.println("Update data!");
		digitalWrite(ledOKPin, LOW);
	}

	if (currentMillis - previousMillis_url >= interval_url) {
		previousMillis_url = currentMillis;

		digitalWrite(ledOKPin, HIGH);

		String name = getFileName();

		Serial.println(name);

		DataLogger::getInstance()->saveData(name, getTimestamp());
		WebClient::getInstance()->sendData(WeatherSensor::getInstance()->toString(), getTimestamp());


		digitalWrite(ledOKPin, LOW);
	}
}


String getFileName(){
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


uint32_t getTimestamp(){
	DateTime now = rtc.now();
	return now.unixtime();
}

