#include <Arduino.h>

// Max - Min value for 8bits / 16bits, signed and unsigned variables
#define UINT16_t_MAX  65536
#define INT16_t_MAX   UINT16_t_MAX/2

#define UINT8_t_MAX 256
#define INT8_t_MAX UINT8_t_MAX/2

// Min - Max value per kind
#define MAX_TEMP 80
#define MIN_TEMP -40

#define MAX_PERC	100
#define MIN_PERC	0

#define MAX_USHORT 50
#define MIN_USHORT 0

#define MAX_ULONG	1100
#define MIN_ULONG	0

class Convertion { 

public:
  Convertion();
  int16_t convertTemperature(float value);
  uint8_t convertPercentage(float value);
  uint8_t convertUShortDecimal(float value);
  uint16_t convertULongDecimal(float value);

  static float constexpr tempConversionFactor = (float)INT16_t_MAX / (float)(MAX_TEMP - MIN_TEMP);
  static float constexpr percConversionFactor = (float)UINT8_t_MAX / (float)(MAX_PERC - MIN_PERC);
  static float constexpr ushortConversionFactor = (float)UINT8_t_MAX / (float)(MAX_USHORT - MIN_USHORT);
  static float constexpr ulongConversionFactor = (float)UINT16_t_MAX / (float)(MAX_ULONG - MIN_ULONG);

private:
};


