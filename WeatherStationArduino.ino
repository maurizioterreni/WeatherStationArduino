#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <RTClib.h>


#include "WeatherSensor.h"


const int chipSelect = 4;

unsigned long previousMillis = 0;
const long interval = 900000; //15min

const int ledOKPin = 10;
const int ledKOPin = 10;

RTC_DS1307 RTC;

void setup() {
  Wire.begin();
  SD.begin(chipSelect);
  RTC.begin();

  if (! RTC.isrunning()) {
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    digitalWrite(ledOKPin, HIGH);
    DateTime now = RTC.now();
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    String dataString = "";
    dataString += String(WeatherSensor::getInstance()->readPressure());
    dataString += String(WeatherSensor::getInstance()->readTemperature(WeatherSensor::BMP));
    dataString += String(WeatherSensor::getInstance()->readTemperature(WeatherSensor::SHT));
    dataString += String(WeatherSensor::getInstance()->readHumidity());
    dataString += String(WeatherSensor::getInstance()->readUv());
    dataString += String(WeatherSensor::getInstance()->readLux());

    String filename = now.year() + now.month() + now.year() + ".txt";

    File dataFile = SD.open(filename, FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      digitalWrite(ledKOPin, LOW);
      // print to the serial port too:
    }
    // if the file isn't open, pop up an error:
    else {
      digitalWrite(ledKOPin, HIGH);
    }
    digitalWrite(ledOKPin, LOW);
  }
}
