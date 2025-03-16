#include <Arduino.h>
#include "sensor.hpp"
#include "relay.hpp"

#if DEV_TYPE == SENSOR_DEV_TYPE
Sensor dev;
#elif DEV_TYPE == RELAY_DEV_TYPE
Relay dev(4, 5);
#endif

void setup()
{
  // Serial.begin(9600); // Serial Monitor
  Config_Init();
  Lora_Init();

  // Serial.println("Device Ready...");
}

void loop()
{
  dev.Loop();
}


