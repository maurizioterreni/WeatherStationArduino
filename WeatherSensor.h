#ifndef WeatherSensor_h
#define WeatherSensor_h

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_HTU21DF.h>
#include <Adafruit_VEML6070.h>
#include <BH1750.h>

class WeatherSensor
{
  public:
    static const int BMP = 0;
    static const int SHT = 1;
    static WeatherSensor* getInstance();
    int32_t readPressure();
    float readTemperature(int id);
    float readHumidity();
    uint16_t readUv();
    uint16_t readLux();
    void updateSensor();
  private:
    static WeatherSensor* instance;
    WeatherSensor();
    void init();
    Adafruit_BMP085 bmp; //Sensore pressione
    Adafruit_HTU21DF htu; //Sensore temperatura
    Adafruit_VEML6070 uv; //Sensore UV
    BH1750 lightMeter; //Sendore Lux

    int32_t pressure;
    float temperature_bmp;
    float temperature_sht;
    float humidity;
    uint16_t lux;
    uint16_t uv_sensor;
    bool bmpIsOn;
};

#endif
