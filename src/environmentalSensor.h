#ifndef UWEATHER_ENVIRONMENTALSENSOR_H
#define UWEATHER_ENVIRONMENTALSENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_BME680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

class EnvironmentalSensor {
public:
    EnvironmentalSensor();
    void init(boolean debug = false);
    boolean performReading(void);
    float getTemperature(void);
    float getPressure(void);
    float getHumidity(void);
    float getGasResistance(void);
    float getAltitude(void);

private:
    Adafruit_BME680 _bme;
};


#endif //UWEATHER_ENVIRONMENTALSENSOR_H
