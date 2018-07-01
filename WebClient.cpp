/*
 * WebClient.cpp
 *
 *  Created on: 29 giu 2018
 *      Author: maurizio
 */

#include "WebClient.h"

WebClient* WebClient::instance = 0;

WebClient* WebClient::getInstance() {
	if (instance == 0){
		instance = new WebClient();
	}

	return instance;
}

void WebClient::sendData(String data, uint32_t timestamp) {
	Serial.println(data + ";" + timestamp);
	char server[] = "www.maurizioterreni.altervista.org";
	if (client.connect(server, 80)) {
		Serial.println("connected");
		// Make a HTTP request:
		client.println("GET /weather/arduino.php?data=" + data + ";" + timestamp + " HTTP/1.1");
		client.println("Host: www.maurizioterreni.altervista.org");
		client.println("Connection: close");
		client.println();
		Serial.println("disconnecting.");
		client.stop();
		
	} else {
		// if you didn't get a connection to the server:
		Serial.println("connection failed");
	}
}

WebClient::WebClient() {
	byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP");
	}
}

uint32_t WebClient::getTimestamp() {
	uint32_t timestamp = 0;

	char server[] = "www.maurizioterreni.altervista.org";
	if (client.connect(server, 80)) {
		Serial.println("connected");
		// Make a HTTP request:
		client.println("GET /getTimeStamp.php HTTP/1.1");
		client.println("Host: www.maurizioterreni.altervista.org");
		client.println("Connection: close");
		client.println();
		long previousMillis = millis();
		while (client.connected()){
			while (client.available()) {
				char c = client.read();
				if(c >= 48 && c <= 57){
					timestamp *= 10;
					timestamp += c - 48;
				}
				Serial.println(c);
			}
			client.stop();
			return 1;
		}


	} else {
		// if you didn't get a connection to the server:
		Serial.println("connection failed");
	}

	return timestamp;
}
