#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>


#include "WeatherSensor.h"
#include "DataLogger.h"
#include "WebClient.h"
#include "TimeSync.h"


String version = "0.4.12";

unsigned long previousMillis_url = millis();
unsigned long previousMillis_update_sensor = millis();
const long interval_url = 900000; //15min
const long interval_update_sensor = 60000; //1min

const int ledOKPin = 13;
const int ledKOPin = 13;

void setup() {
	Wire.begin();

	Serial.begin(9600);

	Serial.println("Weather Station Arduino v" + version);

}

void loop() {
	unsigned long currentMillis = millis();

	if(currentMillis - previousMillis_update_sensor >= interval_update_sensor){
		digitalWrite(ledOKPin, HIGH);
		previousMillis_update_sensor  = currentMillis;
		WeatherSensor::getInstance()->updateSensor();

		Serial.println("Update data!");
		digitalWrite(ledOKPin, LOW);
	}

	if (currentMillis - previousMillis_url >= interval_url) {
		previousMillis_url = currentMillis;

		digitalWrite(ledOKPin, HIGH);

		DataLogger::getInstance()->saveData(TimeSync::getInstance()->getDateAsString(), TimeSync::getInstance()->getTimestamp());

		WebClient::getInstance()->sendData(WeatherSensor::getInstance()->getTemperatureBmp(), "1", "1");
		WebClient::getInstance()->sendData(WeatherSensor::getInstance()->getTemperatureSht(), "1", "2");
		WebClient::getInstance()->sendData(WeatherSensor::getInstance()->getHumidity(), "1", "3");
		WebClient::getInstance()->sendData(WeatherSensor::getInstance()->getLux(), "1", "4");
		WebClient::getInstance()->sendData(WeatherSensor::getInstance()->getPressure(), "1", "5");
		WebClient::getInstance()->sendData(WeatherSensor::getInstance()->getUvSensor(), "1", "6");

		digitalWrite(ledOKPin, LOW);
	}
}

