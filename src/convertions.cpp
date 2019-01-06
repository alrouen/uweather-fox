#include "convertions.h"

Convertion::Convertion() {}

int16_t Convertion::convertTemperature(float value) {

    if(value > MAX_TEMP) {
        return (int16_t)(MAX_TEMP*tempConversionFactor);
    } else if(value < MIN_TEMP) {
        return (int16_t)(MIN_TEMP*tempConversionFactor);
    } else {
        return (int16_t)(value*tempConversionFactor);
    }
}

uint8_t Convertion::convertPercentage(float value) {
    if(value > MAX_PERC) {
        return (uint8_t)(MAX_PERC*percConversionFactor);
    } else if(value < MIN_PERC) {
        return (uint8_t)(MIN_PERC*percConversionFactor);
    } else {
        return (uint8_t)(value*percConversionFactor);
    }
}

uint8_t Convertion::convertUShortDecimal(float value) {
    if(value > MAX_USHORT) {
        return (uint8_t)(MAX_USHORT*ushortConversionFactor);
    } else if(value < MIN_USHORT) {
        return (uint8_t)(MIN_USHORT*ushortConversionFactor);
    } else {
        return (uint8_t)(value*ushortConversionFactor);
    }
}

uint16_t Convertion::convertULongDecimal(float value) {
    if(value > MAX_ULONG) {
        return (uint16_t)(MAX_ULONG*ulongConversionFactor);
    } else if(value < MIN_ULONG) {
        return (uint16_t)(MIN_ULONG*ulongConversionFactor);
    } else {
        return (uint16_t)(value*ulongConversionFactor);
    }
}
