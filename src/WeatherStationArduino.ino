#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//#include <ArduinoJson.h>

#include "WeatherSensor.h"
#include "TimeSync.h"
#include "DataLogger.h"

#define USE_SERIAL Serial

String version = "0.4.12";
String url_server = "http://80.211.86.68:8080/WeatherApi/rest/1.0/measure";

ESP8266WiFiMulti WiFiMulti;

//Intervallo invio dati stazione meteo 15min
unsigned long previousMillis = millis();        // will store last time LED was updated
const long interval = 900000;           // interval at which to blink (milliseconds)
//Fine

//Intervallo lettura dati
unsigned long previousMillis_update_sensor = millis();
const long interval_update_sensor = 60000; //1min
//Fine


ESP8266WebServer server(80); //create a server at port 80

const char* ssid = "";
const char* password = "";

void setup() {

	Wire.begin();

	USE_SERIAL.begin(115200);
	// USE_SERIAL.setDebugOutput(true);

	USE_SERIAL.println();
	USE_SERIAL.println();
	USE_SERIAL.println();

	for (uint8_t t = 4; t > 0; t--) {
		USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
		USE_SERIAL.flush();
		delay(1000);
	}

	USE_SERIAL.print("Version: ");
	USE_SERIAL.println(version);


	TimeSync::getInstance()->begin();


	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	//WiFiMulti.addAP("TIM-29310447", "7hkDyc24hznHl3VuYTrPVJv7");


	USE_SERIAL.print("IP address: ");
	USE_SERIAL.println(WiFi.localIP()); //Print the local IP to access the server

	if (MDNS.begin("WeatherStation")){
    	USE_SERIAL.println("MDNS responder started");
  	}

	server.onNotFound(handleNotFound);
	server.on("/setRtc", handleSetRtc); //Associate the handler function to the path
	server.on("/sensor", handleSensor); //Associate the handler function to the path
	//server.onNotFound(WebServer::getInstance->handleNotFound(&server));	//Pagina 404
  	server.begin();

	USE_SERIAL.println("HTTP server started");
}

void loop() {
	server.handleClient();
	unsigned long currentMillis = millis();
	

	if(currentMillis - previousMillis_update_sensor >= interval_update_sensor){
		previousMillis_update_sensor  = currentMillis;
		//WeatherSensor::getInstance()->updateSensor();

		USE_SERIAL.println(TimeSync::getInstance()->getDateAsString());

	}

	if (currentMillis - previousMillis >= interval) {
		// save the last time you blinked the LED
		previousMillis = currentMillis;
		DataLogger::getInstance()->saveData(TimeSync::getInstance()->getDateAsString(), TimeSync::getInstance()->getTimestamp());

		//httpPost();

	}
}

void handleSetRtc() { //Handler
	String message = "";
	if (server.arg("timestamp") == ""){     //Parameter not found
		message = "Timestamp Argument not found";
	}else{     //Parameter found
		message = "Time is = ";
		unsigned long timestamp = server.arg("timestamp").toInt();
		TimeSync::getInstance()->syncTime(timestamp);
		message += TimeSync::getInstance()->getDateAsString();
	}

	server.send(200, "text/plain", message);          //Returns the HTTP response
}


void handleNotFound() {
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += (server.method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";
	for (uint8_t i = 0; i < server.args(); i++) {
		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
	}
	server.send(404, "text/plain", message);
}


void handleSensor() { //Handler
	USE_SERIAL.print("Sensor");
	String message = "";

	message += "Pressure = ";
	message += String(WeatherSensor::getInstance()->readPressure());
	message += "\n";
	message += "BMP Temperature = ";
	message += String(WeatherSensor::getInstance()->readTemperature(WeatherSensor::BMP));
	message += "\n";
	message += "SHT Temperature = ";
	message += String(WeatherSensor::getInstance()->readTemperature(WeatherSensor::SHT));
	message += "\n";
	message += "Humidity = ";
	message += String(WeatherSensor::getInstance()->readHumidity());
	message += "\n";
	message += "UV = ";
	message += String(WeatherSensor::getInstance()->readUv());
	message += "\n";
	message += "Lux = ";
	message += String(WeatherSensor::getInstance()->readLux());

	server.send(200, "text/plain", message);          //Returns the HTTP response
}



void httpPost(){
	// wait for WiFi connection
	if ((WiFiMulti.run() == WL_CONNECTED)) {

	/*	String body = "[";
		//Sensor id 29
		body += "{\"sensorId\": \"29\",\"quantity\": \"";
		body += WeatherSensor::getInstance()->getTemperatureBmp();
		body += "\",\"unitMeasureId\": \"1\"},";
		//Sensor id 30
		body += "{\"sensorId\": \"30\",\"quantity\": \"";
		body += String(WeatherSensor::getInstance()->getTemperatureSht());
		body += "\",\"unitMeasureId\": \"1\"},";
		//Sensor id 31
		body += "{\"sensorId\": \"31\",\"quantity\": \"";
		body += String(WeatherSensor::getInstance()->getHumidity());
		body += "\",\"unitMeasureId\": \"1\"},";
		//Sensor id 32
	//	body += "{\"sensorId\": \"32\",\"quantity\": \"" + "" + "\",\"unitMeasureId\": \"1\"}," +
		//Sensor id 33
	//	body += "{\"sensorId\": \"33\",\"quantity\": \"" + "" + "\",\"unitMeasureId\": \"1\"}," +
		//Sensor id 34
		body += "{\"sensorId\": \"34\",\"quantity\": \"";
		body += String(WeatherSensor::getInstance()->getPressure());
		body += "\",\"unitMeasureId\": \"1\"},";
		//Sensor id 35
		body += "{\"sensorId\": \"35\",\"quantity\": \"";
		body += String(WeatherSensor::getInstance()->getUvSensor());
		body += "\",\"unitMeasureId\": \"1\"},";
		//Sensor id 36
	//	body += "{\"sensorId\": \"34\",\"quantity\": \"" + "" + "\",\"unitMeasureId\": \"1\"}" +
		body += "]";
*/	
/*		StaticJsonBuffer<200> jsonBuffer;

		JsonObject& jsonObj = jsonBuffer.createObject();
    	char JSONmessageBuffer[200];

		jsonObj["id"] = led_resource.id;

		HTTPClient http;
		http.begin(url_server); //HTTP

		http.addHeader("Accept", "application/json");
		http.addHeader("Content-Type", "application/json");
		int httpCode = http.POST(body);

		// httpCode will be negative on error
		if (httpCode > 0) {
			// HTTP header has been send and Server response header has been handled
			USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

			// file found at server
			if (httpCode == HTTP_CODE_OK) {
				String payload = http.getString();
				USE_SERIAL.println(payload);
			}
		} else {
			USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
			DataLogger::getInstance()->saveError(http.errorToString(httpCode).c_str(), TimeSync::getInstance()->getTimestamp());
		}

		http.end();*/
	}
}
