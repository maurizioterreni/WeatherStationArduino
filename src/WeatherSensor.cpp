#include "WeatherSensor.h"




/* Null, because instance will be initialized on demand. */
WeatherSensor* WeatherSensor::instance = 0;

WeatherSensor* WeatherSensor::getInstance(){
	if (instance == 0){
		instance = new WeatherSensor();
	}

	return instance;
}

WeatherSensor::WeatherSensor(){
	Wire.begin();
	bmpIsOn = false;
	if (bmp.begin()) {
		bmpIsOn = true;
		//Serial.println("test bmp");
	}

	lightMeter.begin();
	uv.begin(VEML6070_1_T);
	htu = Adafruit_HTU21DF();
	htu.begin();

	init();
}


int32_t WeatherSensor::readPressure(){
	if(bmpIsOn){
		return pressure;
	}
	return -1;
}


float WeatherSensor::readTemperature(int id){
	if(id == WeatherSensor::BMP) {
		return temperature_bmp;
	}else if(id == WeatherSensor::SHT) {
		return temperature_sht;
	}else{
		return .0;
	}
}


float WeatherSensor::readHumidity(){
	return humidity;
}


uint16_t WeatherSensor::readUv(){
	return uv_sensor;
}


uint16_t WeatherSensor::readLux(){
	return lux;
}


void WeatherSensor::init(){
	Serial.println("Sensore Init()");
	if(bmpIsOn){
		pressure = bmp.readPressure();
		temperature_bmp = bmp.readTemperature();
	}else{
		pressure = -1;
		temperature_bmp = -200.0;
	}
	temperature_sht = htu.readTemperature();
	humidity = htu.readHumidity();
	lux = lightMeter.readLightLevel();
	uv_sensor = uv.readUV();
}


void WeatherSensor::updateSensor(){
	if(bmpIsOn){
		pressure = (int32_t) ((pressure + bmp.readPressure())/2);
		temperature_bmp = ((temperature_bmp + bmp.readTemperature())/2);
	}else{
		pressure = -1;
		temperature_bmp = -200.0;
	}

	temperature_sht = ((temperature_sht + htu.readTemperature())/2);
	humidity = ((humidity + htu.readHumidity())/2);
	lux = (uint16_t) ((lux + lightMeter.readLightLevel()) / 2);
	uv_sensor = (uint16_t) ((uv_sensor + uv.readUV()) / 2);
}

String WeatherSensor::toString() {
	String dataString = "";
	dataString += String(readPressure());
	dataString += ";";
	dataString += String(readTemperature(WeatherSensor::BMP));
	dataString += ";";
	dataString += String(readTemperature(WeatherSensor::SHT));
	dataString += ";";
	dataString += String(readHumidity());
	dataString += ";";
	dataString += String(readUv());
	dataString += ";";
	dataString += String(readLux());

	return dataString;
}
