#ifndef _RELAY_H_
#define _RELAY_H_

#include "Arduino.h"
#include "lora.h"

using namespace std;

#define RELAY_ON 1
#define RELAY_OFF 0

class Relay
{
private:
    bool relay1;
    bool relay2;
    int relay1_pin;
    int relay2_pin;


    void updateState();

public:
    Relay(int relay1_pin, int relay2_pin);
    ~Relay();

    void Loop();
};

Relay::Relay(int relay1_pin, int relay2_pin)
{
    this->relay1_pin = relay1_pin;
    this->relay2_pin = relay2_pin;

    pinMode(relay1_pin, OUTPUT);
    pinMode(relay1_pin, OUTPUT);
}

Relay::~Relay()
{
}

void Relay::Loop()
{
    Frame_t frame;
    if (Lora_Receive(&frame))
    {
        if (frame.devId == DEV_ID)
        {
            bool isResponse = true;
            for (int i = 0; i < frame.commandSize; i++)
            {
                switch (frame.commands[i].cmd)
                {
                case Relay1Get:
                    frame.commands[i].data[0] = relay1;
                    break;
                case Relay1Set:
                    relay1 = frame.commands[i].data[0];
                    break;
                case Relay2Get:
                    frame.commands[i].data[0] = relay2;
                    break;
                case Relay2Set:
                    relay2 = frame.commands[i].data[0];
                    break;
                default:
                    isResponse = false;
                }
            }

            updateState();

            // Send response
            if (isResponse)
            {
                Lora_Send(frame);
            }
        }
    }
}

void Relay::updateState()
{
  digitalWrite(relay1_pin, relay1);
  digitalWrite(relay2_pin, relay2);
}

#endif