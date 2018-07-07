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

unsigned long WebClient::syncTimeNTP() {
  if(Udp.begin(8888) == 0) {
    Serial.println("Failed to configure UDP");
  }
	char timeServer[] = "time.nist.gov";
	byte packetBuffer[ 48 ];

	memset(packetBuffer, 0, 48);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(timeServer, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, 48);
  Udp.endPacket();

  Serial.println(timeServer);

	// wait to see if a reply is available
  delay(1000);
  if ( Udp.parsePacket() ) {
    // We've received a packet, read the data from it
    Udp.read(packetBuffer, 48); // read the packet
                                             // into the buffer
    //the timestamp starts at byte 40 of the received packet
    // and is four bytes, or two words, long. First,
    // esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);


    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich
                                            // Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour
                                            // (86400 equals secs per day)
    Serial.print(':');
    if ( ((epoch % 3600) / 60) < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute
                                        // (3600 equals secs per minute)
    Serial.print(':');
    if ( (epoch % 60) < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second

		return epoch;
  }
  // wait ten seconds before asking for the time again

  return 0;
}

WebClient::WebClient() {
	Serial.println("Config WebClient");
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
