#include "WeatherSensor.h"




/* Null, because instance will be initialized on demand. */
WeatherSensor* WeatherSensor::instance = 0;

WeatherSensor* WeatherSensor::getInstance()
{
	if (instance == 0)
	{
		instance = new WeatherSensor();
	}

	return instance;
}

WeatherSensor::WeatherSensor()
{
  bmp.begin();
  uv.begin(VEML6070_1_T);
}


int32_t WeatherSensor::readPressure(){
  return bmp.readPressure();
}


float WeatherSensor::readTemperature(int id){
  if(id == WeatherSensor::BMP) {
    return bmp.readTemperature();
  }else if(id == WeatherSensor::SHT) {
    return sht.getTemperature();
  }else{
	return .0;
  }
}


float WeatherSensor::readHumidity(){
  return sht.getHumidity();
}


uint16_t WeatherSensor::readUv(){
  return uv.readUV();
}


uint16_t WeatherSensor::readLux(){
  return lightMeter.readLightLevel();
}
