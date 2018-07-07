/*
 * WebClient.h
 *
 *  Created on: 29 giu 2018
 *      Author: maurizio
 */

#ifndef WEBCLIENT_H_
#define WEBCLIENT_H_

#include "Arduino.h"
#include <HardwareSerial.h>
#include <SoftwareSerial.h>

class WebClient {
public:
	static WebClient* getInstance();
	unsigned long syncTimeNTP();
	uint32_t getTimestamp();
	void sendData(float data, String unitMeasure, String sensorId);
	void sendData(String data, String unitMeasure, String sensorId);
	void sendData(uint16_t data, String unitMeasure, String sensorId);
	void sendData(int32_t data, String unitMeasure, String sensorId);
private:
	void sendPostData(String quantity, String unitMeasure, String sensorId);
	void check_status(int statusCode);
	void check_response(String response);
	WebClient();
	static WebClient* instance;
	boolean connectWiFi();
	void WifiConfig();
	String ssid;
	String pass;
	String dstIp;
};

#endif /* WEBCLIENT_H_ */
