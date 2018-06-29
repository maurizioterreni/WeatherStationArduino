/*
 * DataLogger.h
 *
 *  Created on: 28 giu 2018
 *      Author: maurizio
 */

#ifndef DATALOGGER_H_
#define DATALOGGER_H_

#include <SPI.h>
#include <SD.h>
#include "WeatherSensor.h"


class DataLogger {
public:
	void saveData(String fileName);
	static DataLogger* getInstance();
	const int CHIP_SELECT = 5;
private:
	static DataLogger* instance;
	bool isSdActive;
	DataLogger();
};


#endif /* DATALOGGER_H_ */
