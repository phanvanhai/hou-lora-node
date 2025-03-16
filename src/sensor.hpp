#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "Arduino.h"
#include "lora.h"

using namespace std;

class Sensor
{
private:
    int8_t temperature;
    uint8_t humidity;
    uint8_t soil;
    uint16_t lux;

    unsigned long lastReportTimeMs;

    void read();
    Frame_t genReportFrame();

public:
    Sensor(/* args */);
    ~Sensor();

    void Loop();
};

Sensor::Sensor(/* args */)
{
    lastReportTimeMs = 0;
}

Sensor::~Sensor()
{
}

void Sensor::Loop()
{
    // Khong xu ly nhan Frame

    // Report
    if (REPORT_INTERVAL_MS && (millis() - lastReportTimeMs >= REPORT_INTERVAL_MS))
    {
        lastReportTimeMs = millis();
        read();
        Lora_Send(genReportFrame());
    }
}

void Sensor::read()
{
    // random for test
    temperature = int8_t(random(30, 40));
    humidity = uint8_t(random(60, 80));
    soil = uint8_t(random(50, 70));
    lux = uint16_t(random(300, 600));;
}

Frame_t Sensor::genReportFrame()
{
    Frame_t frame;

    frame.devId = DEV_ID;
    frame.commandSize = 4;

    frame.commands[0].cmd = TemperatureReport;
    frame.commands[0].dataSize = (uint8_t)(Config_GetSizeByCmd(TemperatureReport));
    frame.commands[0].data[0] = temperature;

    frame.commands[1].cmd = HumidityReport;
    frame.commands[1].dataSize = (uint8_t)(Config_GetSizeByCmd(HumidityReport));
    frame.commands[1].data[0] = humidity;

    frame.commands[2].cmd = SoilReport;
    frame.commands[2].dataSize = (uint8_t)(Config_GetSizeByCmd(SoilReport));
    frame.commands[2].data[0] = lux;

    frame.commands[3].cmd = LuxReport;
    frame.commands[3].dataSize = (uint8_t)(Config_GetSizeByCmd(LuxReport));
    frame.commands[3].data[0] = (lux >> 8) & 0xFF;
    frame.commands[3].data[1] = lux & 0xFF;

    return frame;
}

#endif