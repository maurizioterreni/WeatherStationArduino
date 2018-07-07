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


WebClient::WebClient() {
	Serial.println("Config WebClient");

	Serial1.begin(9600);
	Serial1.setTimeout(1000);

	ssid = "";
	pass = "";
	dstIp = "";
}

void WebClient::WifiConfig(){
	char ready[] = "ready";
	char readyUpper[] = "Ready";
	while(Serial1.available()>0)
		Serial1.read();

	delay(1000);
	//test if the module is ready
	Serial1.println("AT+RST");
	//delay(1000);
	//delay(1000);
	Serial.println("Resetting module");
	Serial1.flush();

	//if(Serial2.find("ready"))
	if(Serial1.find(ready)||Serial1.find(readyUpper))
	{
		//dbgSerial.println("Module is ready");
		Serial.println("Module is ready");
	}
	else
	{
		//dbgSerial.println("Module have no response.");
		Serial.println("Module have no response.");
	}
	delay(1000);
	//connect to the wifi
	boolean connected=false;
	for(int i=0;i<5;i++)
	{
		if(connectWiFi())
		{
			connected = true;
			break;
		}
	}
	if (!connected){while(1);}
	delay(5000);
	//print the ip addr
	/*
		 Serial2.println("AT+CIFSR");
	     Serial.println("ip address:");
	     while (Serial2.available())
	     Serial.write(Serial2.read());

	 */
	//set the single connection mode
	Serial1.println("AT+CIPMUX=0");
}


boolean WebClient::connectWiFi(){
	char ok[] = "OK";

	Serial1.println("AT+CWMODE=1");
	String cmd="AT+CWJAP=\"";
	cmd+=ssid;
	cmd+="\",\"";
	cmd+=pass;
	cmd+="\"";
	//dbgSerial.println(cmd);
	Serial1.println(cmd);
	Serial.println(cmd);
	delay(2000);
	if(Serial1.find(ok))
	{
		//dbgSerial.println("OK, Connected to WiFi.");
		Serial.println("OK, Connected to WiFi.");
		return true;
	}else
	{
		//dbgSerial.println("Can not connect to the WiFi.");
		Serial.println("Can not connect to the WiFi.");
		return false;
	}
}

void WebClient::sendData(float data, String unitMeasure, String sensorId){
	sendPostData(String(data), unitMeasure, sensorId);
}


void WebClient::sendData(String data, String unitMeasure, String sensorId){
	sendPostData(data, unitMeasure, sensorId);
}

void WebClient::sendData(uint16_t data, String unitMeasure, String sensorId){
	sendPostData(String(data), unitMeasure, sensorId);
}
void WebClient::sendData(int32_t data, String unitMeasure, String sensorId){
	sendPostData(String(data), unitMeasure, sensorId);
}


void WebClient::sendPostData(String quantity, String unitMeasure, String sensorId){
	char error[] = "Error";
	char major[] = ">";
	String cmd = "AT+CIPSTART=\"TCP\",\"";
	cmd += dstIp;
	cmd += "\",8080";
	Serial1.println(cmd);
	//dbgSerial.println(cmd);
	Serial.println(cmd);
	if(Serial1.find(error)) return;
	cmd = "POST /rest/1.0/measure HTTP/1.0 \r\n Content-Type: application/json\r\n  {\"sensorId\": \""+ sensorId +"\", \"quantity\": \"" + quantity +"\",\"unitMeasureId\": \" "+ unitMeasure +" \",}";
	Serial1.print("AT+CIPSEND=");
	Serial1.println(cmd.length());
	if(Serial2.find(major))
	{
		//dbgSerial.print(">");
		Serial.print(major);
	}else
	{
		Serial1.println("AT+CIPCLOSE");
		//dbgSerial.println("connect timeout");
		Serial.println("connect timeout");
		delay(1000);
		return;
	}
	Serial1.print(cmd);
	delay(2000);
	//Serial.find("+IPD");
	while (Serial1.available())
	{
		char c = Serial1.read();
		//dbgSerial.write(c);
		Serial.write(c);
		//if(c=='\r') dbgSerial.print('\n');
		if(c=='\r') Serial.print('\n');
	}
	//dbgSerial.println("====");
	Serial.println("====");
	delay(1000);
}


void WebClient::check_status(int statusCode){
	delay(1000);
	if(statusCode == 200){
		Serial.print("TEST RESULT: ok (");
		Serial.print(statusCode);
		Serial.println(")");
	}else{
		Serial.print("TEST RESULT: fail (");
		Serial.print(statusCode);
		Serial.println(")");
	}
}

void WebClient::check_response(String response){
	//Serial.println(response);
	if(response == "OK"){
		Serial.println("TEST RESULT: ok (response body)");
	}else{
		Serial.println("TEST RESULT: fail (response body = " + response + ")");
	}
	response = "";
}
