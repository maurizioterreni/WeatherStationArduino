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

	client.dhcp();


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
	String response;
	String post_body = "{\"sensorId\": \""+ sensorId +"\", \"quantity\": \"" + quantity +"\",\"unitMeasureId\": \" "+ unitMeasure +" \",}";

	client.setHeader("Accept: application/json");
	client.setHeader("Content-Type: application/json");
	check_status(client.post("/data", post_body.buffer, &response));
	check_response(response);
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
