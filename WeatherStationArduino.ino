#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <WiFiEsp.h>


#include <SoftwareSerial.h>
SoftwareSerial Serial1(6, 7); // RX, TX



#include "WeatherSensor.h"

char ssid[] = "Twim";            // your network SSID (name)
char pass[] = "12345678";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "www.maurizioterreni.altervista.org";

// Initialize the Ethernet client object
WiFiEspClient client;

unsigned long previousMillis = 0;
const long interval = 900000; //15min

const int ledOKPin = 13;
const int ledKOPin = 13;

void setup() {
	Wire.begin();

	Serial1.begin(9600);
	// initialize ESP module
	WiFi.init(&Serial1);

	// check for the presence of the shield
	if (WiFi.status() == WL_NO_SHIELD) {
		while (true);
	}

	// attempt to connect to WiFi network
	while ( status != WL_CONNECTED) {
		// Connect to WPA/WPA2 network
		status = WiFi.begin(ssid, pass);
	}


}

void loop() {
	unsigned long currentMillis = millis();


	if (currentMillis - previousMillis >= interval) {
		previousMillis = currentMillis;

		digitalWrite(ledOKPin, HIGH);

		if (client.connect(server, 80)) {
			Serial.println("Connected to server");
			// Make a HTTP request
			client.println(getStrData());
			client.println("Host: www.maurizioterreni.altervista.org");
			client.println("Connection: close");
			client.println();
		}

		unsigned long timeout = millis();
		while (client.available() == 0) {
			if (millis() - timeout > 5000) {
				Serial.println(">>> Client Timeout !");
				client.stop();
				return;
			}
		}

		digitalWrite(ledOKPin, LOW);
	}
}


String getStrData() {
	String dataString = "GET /weather/arduino.php?data=";
	dataString += String(WeatherSensor::getInstance()->readPressure());
	dataString += ";";
	dataString += String(WeatherSensor::getInstance()->readTemperature(WeatherSensor::BMP));
	dataString += ";";
	dataString += String(WeatherSensor::getInstance()->readTemperature(WeatherSensor::SHT));
	dataString += ";";
	dataString += String(WeatherSensor::getInstance()->readHumidity());
	dataString += ";";
	dataString += String(WeatherSensor::getInstance()->readUv());
	dataString += ";";
	dataString += String(WeatherSensor::getInstance()->readLux());
	dataString += "  HTTP/1.1";


	return const_cast<char*>(dataString.c_str());

}
