/*
 * WebClient.h
 *
 *  Created on: 29 giu 2018
 *      Author: maurizio
 */

#ifndef WEBCLIENT_H_
#define WEBCLIENT_H_

#include "Arduino.h"
#include <SPI.h>
#include <Ethernet.h>

class WebClient {
public:
	static WebClient* getInstance();
	void sendData(String data, uint32_t timestamp);
	uint32_t getTimestamp();
private:
	WebClient();
	static WebClient* instance;

	EthernetClient client;
};

#endif /* WEBCLIENT_H_ */
