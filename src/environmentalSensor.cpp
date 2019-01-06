#include "environmentalSensor.h"

EnvironmentalSensor::EnvironmentalSensor() : _bme() {
}

void EnvironmentalSensor::init(boolean debug) {
    if (debug) {
        Serial.println("Environmental sensor init and test");
    }

    if (!_bme.begin()) {
        if (debug) {
            Serial.println("Could not find a valid BME680 sensor, check wiring!");
        }
        while (1);
    }

    // Set up oversampling and filter initialization
    _bme.setTemperatureOversampling(BME680_OS_8X);
    _bme.setHumidityOversampling(BME680_OS_2X);
    _bme.setPressureOversampling(BME680_OS_4X);
    _bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    _bme.setGasHeater(320, 150); // 320*C for 150 ms
}

boolean EnvironmentalSensor::performReading(void) {
    return _bme.performReading();

}

float EnvironmentalSensor::getTemperature(void) {
    return _bme.temperature;
}

float EnvironmentalSensor::getPressure(void) {
    return _bme.pressure;
}

float EnvironmentalSensor::getHumidity(void) {
    return _bme.humidity;
}

float EnvironmentalSensor::getGasResistance(void) {
    return _bme.gas_resistance;
}

float EnvironmentalSensor::getAltitude(void) {
    return _bme.readAltitude(SEALEVELPRESSURE_HPA);
}
