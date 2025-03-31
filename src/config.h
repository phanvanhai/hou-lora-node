#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <Arduino.h>

// ================ Dev Address ==================
#define DEV_ID 0xAABBCCDDEE01


// ================ Dev Type ==================
#define SENSOR_DEV_TYPE 0
#define RELAY_DEV_TYPE 1

#define DEV_TYPE RELAY_DEV_TYPE


// ================ Interval for reporting sensor data ==================
#define REPORT_INTERVAL_MS 5000    // ms. 0 <-> No report

// ================ Config pin ==================
#define LORA_TX_PIN 2
#define LORA_RX_PIN 3

// ================ List CMD ==================
typedef enum {
    TX_DIR,
    RX_DIR
} DataDirection_t;

typedef enum
{
    TemperatureReport = 0x11,
    HumidityReport = 0x12,
    SoilReport = 0x13,
    LuxReport = 0x14,

    Relay1Report = 0x30,
    Relay1Get = 0x31,
    Relay1Set = 0x32,

    Relay2Report = 0x33,
    Relay2Get = 0x34,
    Relay2Set = 0x35
} Cmd_t;

void Config_Init();
int8_t Config_GetSizeByCmd(uint8_t cmd, DataDirection_t dir);

#endif
