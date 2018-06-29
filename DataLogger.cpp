/*
 * DataLogger.cpp
 *
 *  Created on: 28 giu 2018
 *      Author: maurizio
 */

#include "DataLogger.h"

DataLogger* DataLogger::instance = 0;

DataLogger* DataLogger::getInstance() {
	if (instance == 0){
		instance = new DataLogger();
	}

	return instance;
}

void DataLogger::saveData(String fileName) {
	if(!isSdActive){
		return;
	}

	File dataFile = SD.open(fileName + ".wth", FILE_WRITE);

	if (dataFile) {
		dataFile.println(WeatherSensor::getInstance()->toString() + "\n");
		dataFile.close();
	}

	else {
		isSdActive = false;
	}
}

DataLogger::DataLogger() {
	if (!SD.begin(CHIP_SELECT)) {
		isSdActive = false;
		Serial.println("SD not connected!");

	}else{
		isSdActive = true;
		Serial.println("SD connected!");
	}
}


