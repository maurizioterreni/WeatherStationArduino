/*#ifndef RestClient_h
#define RestClient_h

#include "Arduino.h"
#include "WeatherSensor.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>


class RestClient
{
  public:
    static RestClient* getInstance();
    int httpClient();
  private:
    static const char* SSID     = "xxxx";      // SSID
    static const char* PASSWORD = "xxxx";     //  Password
    static const char* HOST     = "xxx.xxx.xxx.xxx"; 
    static const char* END_POINT= "/xxxxx";
    static int PORT             = 8080;       //  PORT
    static RestClient* instance;
    ESP8266WiFiMulti wifi;
    HTTPClient http;
    WeatherSensor* weatherSensor;
    RestClient();
};

#endif*/