#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <EtherCard.h>


#include "WeatherSensor.h"

#define STATUS_IDLE    0
#define STATUS_SENT    1
#define TIMEOUT        5000


static byte mymac[] = {0xDD,0xDD,0xDD,0x00,0x00,0x01};
byte Ethernet::buffer[700];

const char website[] PROGMEM = "www.maurizioterreni.altervista.org";
static byte session_id;
byte actual_status;


unsigned long previousMillis = 0;
const long interval = 900000; //15min

const int ledOKPin = 10;
const int ledKOPin = 10;

void setup() {
	Wire.begin();


	ether.begin(sizeof Ethernet::buffer, mymac, 10);

	ether.dhcpSetup();

	ether.dnsLookup(website);
}

void loop() {
	ether.packetLoop(ether.packetReceive());
	unsigned long currentMillis = millis();

	if(actual_status == STATUS_IDLE){
		if (currentMillis - previousMillis >= interval) {
			digitalWrite(ledOKPin, HIGH);
			previousMillis = currentMillis;
			sendData();
			digitalWrite(ledOKPin, LOW);
		}
	}else if(actual_status == STATUS_SENT) {
		if(currentMillis - previousMillis > TIMEOUT) {
			previousMillis = currentMillis;
			actual_status = STATUS_IDLE;
		}
		checkResponse();
	}
}


void sendData() {
	String dataString = "";
	dataString += String(WeatherSensor::getInstance()->readPressure());
	dataString += "#";
	dataString += String(WeatherSensor::getInstance()->readTemperature(WeatherSensor::BMP));
	dataString += "#";
	dataString += String(WeatherSensor::getInstance()->readTemperature(WeatherSensor::SHT));
	dataString += "#";
	dataString += String(WeatherSensor::getInstance()->readHumidity());
	dataString += "#";
	dataString += String(WeatherSensor::getInstance()->readUv());
	dataString += "#";
	dataString += String(WeatherSensor::getInstance()->readLux());

	Stash stash;
	byte sd = stash.create();
	stash.print(dataString);
	stash.save();

	Stash::prepare(PSTR("GET /weather/arduino.php?data=$H HTTP/1.0" "\r\n"
			"Host: $F" "\r\n" "\r\n"),
			sd, website);
	session_id = ether.tcpSend();

	actual_status = STATUS_SENT;
}

void checkResponse() {

	const char* reply = ether.tcpReply(session_id);
	if(reply > 0) {
		if(strstr(reply, "KO - ") != 0){}
		else Serial.println("OK");
		actual_status = STATUS_IDLE;
	}
}
