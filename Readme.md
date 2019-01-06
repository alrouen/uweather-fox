## Sigfox/AWS callback configuration :

- topic: 

any topic you want to configure. You just need to configure a AWS IoT Core rule to match this topic. I.e. if your topic is 'uweather', you rule must be 'SELECT * FROM "uweather"' to catch message and then to what you want with it.

- custom payload configuration: 

    ```
    voltage::uint:16:little-endian humidity::uint:8:little-endian temperature::int:16:little-endian internalHumidity::uint:8:little-endian internalTemp::int:16:little-endian internalPressure::uint:16:little-endian internalGasResistance::uint:16:little-endian
    ```

- json body configuration:

    ```
    {
        "device": "{device}",
        "time": {time},
        "data": "{data}",
        "duplicate": {duplicate},
        "voltage" : {customData#voltage},
        "humidity" : {customData#humidity},
        "temperature" : {customData#temperature},
        "internalHumidity" : {customData#internalHumidity,
        "internalTemp" : {customData#internalTemp},
        "internalPressure" : {customData#internalPressure},
        "internalGasResistance" : {customData#internalGasResistance}
    }
    ```

