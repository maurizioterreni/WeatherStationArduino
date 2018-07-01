/*
 * WebClient.cpp
 *
 *  Created on: 29 giu 2018
 *      Author: maurizio
 */

#include "WebClient.h"

WebClient* WebClient::instance = 0;
unsigned int WebClient::localPort = 8888;
byte WebClient::timeServer[] = {193,5,216,14};//{193, 79, 237, 14};

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


	sendNTPpacket(timeServer);

	// wait to see if a reply is available
	delay(1000);


	if ( Udp.available() ) {

		Udp.read(pb, 48);


		// NTP contains four timestamps with an integer part and a fraction part
		// we only use the integer part here
		unsigned long t4 = 0;
		for (int i=0; i< 4; i++)
		{
			t4 = t4 << 8 | pb[40+i];
		}

		// part of the fractional part
		// could be 4 bytes but this is more precise than the 1307 RTC
		// which has a precision of ONE second
		// in fact one byte is sufficient for 1307
		float f4 = ((long)pb[44] * 256 + pb[45]) / 65536.0;

		// NOTE:
		// one could use the fractional part to set the RTC more precise
		// 1) at the right (calculated) moment to the NEXT second!
		//    t4++;
		//    delay(1000 - f4*1000);
		//    RTC.adjust(DateTime(t4));
		//    keep in mind that the time in the packet was the time at
		//    the NTP server at sending time so one should take into account
		//    the network latency (try ping!) and the processing of the data
		//    ==> delay (850 - f4*1000);
		// 2) simply use it to round up the second
		//    f > 0.5 => add 1 to the second before adjusting the RTC
		//   (or lower threshold eg 0.4 if one keeps network latency etc in mind)
		// 3) a SW RTC might be more precise, => ardomic clock :)


		// convert NTP to UNIX time, differs seventy years = 2208988800 seconds
		// NTP starts Jan 1, 1900
		// Unix time starts on Jan 1 1970.
		const unsigned long seventyYears = 2208988800UL;


		t4 -= seventyYears;



		t4 -= (3 * 3600L);     // Notice the L for long calculations!!
		t4 += 1;               // adjust the delay(1000) at begin of loop!
		if (f4 > 0.4) t4++;    // adjust fractional part, see above

		return t4;
	}else{
		Serial.println("No UDP available ...");
	}

	return 0;
}

void WebClient::sendNTPpacket(byte* address) {
	// set all bytes in the buffer to 0
	memset(pb, 0, 48);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	pb[0] = 0b11100011;   // LI, Version, Mode
	pb[1] = 0;     // Stratum, or type of clock
	pb[2] = 6;     // Polling Interval
	pb[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	pb[12]  = 49;
	pb[13]  = 0x4E;
	pb[14]  = 49;
	pb[15]  = 52;

	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	// IDE 1.0 compatible:
	Udp.beginPacket(address, 123); //NTP requests are to port 123
	Udp.write(pb,48);
	Udp.endPacket();

}

WebClient::WebClient() {
	byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP");
	}

	Udp.begin(localPort);
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
