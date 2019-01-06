#include <Arduino.h>
#include <Wire.h>
#include <SigFox.h>
#include <ArduinoLowPower.h>
#include "environmentalSensor.h"
#include "sht20.h"
#include "convertions.h"

// Collect interval (in milliseconds)
#define COLLECT_INTERVAL 900000 // every 15 mins

// Convertion class
Convertion convertion = Convertion();

// SHT20 - 0x40 
SHT20 sht20 = SHT20();

// BME680 - 0x77
EnvironmentalSensor myEnvSensor = EnvironmentalSensor();

/*
    ATTENTION !!
    - the structure is made to match SigFox size limitation (12 bytes!!!)
    - the structure we are going to send MUST
    be declared "packed" otherwise we'll get padding mismatch
    on the sent data - see http://www.catb.org/esr/structure-packing/#_structure_alignment_and_padding
    for more details
*/
typedef struct __attribute__ ((packed)) sigfox_message {
    uint16_t battery;
    uint8_t sht20_humidity;
    int16_t sht20_temperature;
    uint8_t bme680_humidity;
    int16_t bme680_temperature;
    uint16_t bme680_pressure;
    uint16_t bme680_gasResistance;
} SigfoxMessage;

// stub for message which will be sent
SigfoxMessage msg;

void reboot() {
  NVIC_SystemReset();
  while (1);
}

void PrintHex8(uint8_t *data, uint8_t length) // prints 8-bit data in hex
{
     char tmp[length*5+1];
     byte first;
     byte second;
     for (int i=0; i<length; i++) {
           first = (data[i] >> 4) & 0x0f;
           second = data[i] & 0x0f;
           // base for converting single digit numbers to ASCII is 48
           // base for 10-16 to become upper-case characters A-F is 55
           // note: difference is 7
           tmp[i*5] = 48; // add leading 0
           tmp[i*5+1] = 120; // add leading x
           tmp[i*5+2] = first+48;
           tmp[i*5+3] = second+48;
           tmp[i*5+4] = 32; // add trailing space
           if (first > 9) tmp[i*5+2] += 7;
           if (second > 9) tmp[i*5+3] += 7;
     }
     tmp[length*5] = 0;
     Serial.println(tmp);
}

void sendData(
    float battery, 
    float sht20_humidity,
    float sht20_temperature,
    float bme680_humidity,
    float bme680_temperature,
    float bme680_pressure,
    float bme680_gasResistance) {

    // Start the module
    SigFox.begin();
    // Wait at least 30ms after first configuration (100ms before)
    delay(100);

    msg.battery = convertion.convertULongDecimal(battery);
    msg.sht20_humidity = convertion.convertPercentage(sht20_humidity);
    msg.sht20_temperature = convertion.convertTemperature(sht20_temperature);
    msg.bme680_humidity = convertion.convertPercentage(bme680_humidity);
    msg.bme680_temperature = convertion.convertTemperature(bme680_temperature);
    msg.bme680_pressure = convertion.convertULongDecimal(bme680_pressure);
    msg.bme680_gasResistance = convertion.convertULongDecimal(bme680_gasResistance);

    // We can only read the module temperature before SigFox.end()
    // But so far I don't want to send this info
    //msg.internalTemperature = convertion.convertTemperature(SigFox.internalTemperature());
    
    // Clears all pending interrupts
    SigFox.status();
    delay(1);

    // Send message 
    SigFox.beginPacket();
    SigFox.write((uint8_t*)&msg, 12);
    PrintHex8((uint8_t*)&msg, 12);

    // End transmission
    int ret = SigFox.endPacket(true);  // send buffer to SIGFOX network and wait for a response
    if (ret > 0) {
        Serial.println("No transmission");
    } else {
        Serial.println("Transmission ok");
    }

    // Check ack
    Serial.println(SigFox.status(SIGFOX));
    Serial.println(SigFox.status(ATMEL));

    if (SigFox.parsePacket()) {
        Serial.println("Response from server:");
        while (SigFox.available()) {
        Serial.print("0x");
        Serial.println(SigFox.read(), HEX);
        }
    } else {
        Serial.println("Could not get any response from the server");
        Serial.println("Check the SigFox coverage in your area");
        Serial.println("If you are indoor, check the 20dB coverage or move near a window");
    }
    Serial.println();

    SigFox.end();
}

void donothing() { 
    Serial.println("Woke up!");
}

void setup() {

    Serial.begin(9600);
    // while (!Serial);
    myEnvSensor.init(true); // environment sensor with debug mode

    sht20.initSHT20();                                
    delay(100);
    sht20.checkSHT20();

    LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, donothing, CHANGE);

    if (!SigFox.begin()) {
        Serial.println("Shield error or not present!");
        reboot();
    }

    // Enable debug led and disable automatic deep sleep
    // Comment this line when shipping your project :)
    SigFox.debug();

    String version = SigFox.SigVersion();
    String ID = SigFox.ID();
    String PAC = SigFox.PAC();

    // Display module informations
    Serial.println("MKRFox1200 module information:");
    Serial.println("SigFox FW version " + version);
    Serial.println("ID  = " + ID);
    Serial.println("PAC = " + PAC);
    Serial.println("----");
    delay(100);
    // Send the module to the deepest sleep
    SigFox.end();
}

float readBatteryVoltage() {
    analogReadResolution(10);
    analogReference(AR_INTERNAL1V0); //AR_DEFAULT: the default analog reference of 3.3V // AR_INTERNAL1V0: a built-in 1.0V reference
    // read the input on analog pin 0:
    int sensorValue = analogRead(ADC_BATTERY);
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 4.3V):
    float voltage = sensorValue * (3.25 / 1023.0);
    return voltage;
}

void loop() {
    float batteryVoltage = readBatteryVoltage();
    float sht20Humd = sht20.readHumidity();       
    float sht20Temp = sht20.readTemperature();

    if(myEnvSensor.performReading()) {
        sendData(
            batteryVoltage,
            sht20Humd,
            sht20Temp,
            myEnvSensor.getHumidity(),
            myEnvSensor.getTemperature(),
            myEnvSensor.getPressure()/100.0,
            myEnvSensor.getGasResistance()/1000.0
        );
    } else {
        sendData(
            batteryVoltage,
            sht20Humd,
            sht20Temp,
            0.0,
            0.0,
            0.0,
            0.0
        );
    }

    // Go into sleep mode
    LowPower.sleep(COLLECT_INTERVAL);
    //delay(COLLECT_INTERVAL);
}
