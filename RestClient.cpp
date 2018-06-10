/*#include "RestClient.h"




RestClient* RestClient::instance = 0;

RestClient* RestClient::getInstance()
{
    if (instance == 0)
    {
        instance = new RestClient();
    }

    return instance;
}

RestClient::RestClient()
{
  weatherSensor = WeatherSensor::getInstance();
  wifi.addAP(SSID, PASSWORD);
}


int RestClient::httpClient(){
  if (wifiMulti.run() != WL_CONNECTED) {
      return -1;
  }
  
  http.begin(host,port,url);
  int httpCode = http.GET();
  http.end();
  WiFi.disconnect();
  
  return httpCode;
}*/