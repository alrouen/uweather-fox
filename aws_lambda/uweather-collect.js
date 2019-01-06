// Lambda sample to read uweather external station payload
const UINT8MAX = 256;
const INT8MAX = UINT8MAX/2;
const UINT16MAX = 65536;
const INT16MAX = UINT16MAX/2;

const TEMP_MAX = 80;
const TEMP_MIN = -40;
const PERC_MAX = 100;
const PERC_MIN = 0;
const ULONG_MAX = 1100;
const ULONG_MIN = 0;
const USHORT_MAX = 50;
const USHORT_MIN = 0;

const TEMP_CONV_FACTOR = INT16MAX / (TEMP_MAX - TEMP_MIN);
const PERC_CONV_FACTOR = UINT8MAX / (PERC_MAX - PERC_MIN);
const ULONG_CONV_FACTOR = UINT16MAX / (ULONG_MAX - ULONG_MIN);
const USHORT_CONV_FACTOR = UINT8MAX / (USHORT_MAX - USHORT_MIN);

const convertTemperature = (value) => {
    return (value / TEMP_CONV_FACTOR).toFixed(2);
};

const convertPerc = (value) => {
    return (value / PERC_CONV_FACTOR).toFixed(2);
};

const convertULong = (value) => {
    return (value / ULONG_CONV_FACTOR).toFixed(2);
};

const convertUShort = (value) => {
    return (value / USHORT_CONV_FACTOR).toFixed(2);
};

exports.handler = async (event) => {
    
    //console.log(event);
    const date = new Date(event.time*1000);
    const voltage = convertULong(event.voltage);
    const humidity = convertPerc(event.humidity);
    const temperature = convertTemperature(event.temperature);
    const internalHumidity = convertPerc(event.internalHumidity);
    const internalTemp = convertTemperature(event.internalTemp);
    const internalPressure = convertULong(event.internalPressure);
    const internalGasResistance = convertULong(event.internalGasResistance);
    const message = `${date}: ${voltage} V / ${humidity} % / ${temperature} °C / ${internalHumidity} % / ${internalTemp} °C / ${internalPressure} hPa / ${internalGasResistance} kOhm`;
    console.log(message);
    
    // TODO implement
    const response = {
        statusCode: 200,
        body: JSON.stringify('Hello from Lambda!'),
    };
    return response;
};
