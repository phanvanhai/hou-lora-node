#include "lora.h"
#include <SoftwareSerial.h>

// =============== PACKET func: Begin ==================
#define MAX_PAYLOAD_SIZE (MAX_COMMAND_SIZE * MAX_COMMAND_DATA_SIZE)
#define MIN_PAYLOAD_SIZE 6  // 6 byte MAC
#define HEADER_BYTE 0x55

typedef enum
{
    RxHeader = 0x00,
    RxLen,
    RxPayload
} RxState_t;

typedef struct
{
    uint8_t len;
    uint8_t payload[MAX_PAYLOAD_SIZE];
} Packet_t;

static RxState_t rxState = RxHeader;
static Packet_t g_packet;
static uint8_t payload_cnt = 0;

bool Packet_Read(SoftwareSerial &serial, Packet_t *result_packet);
void Packet_Send(SoftwareSerial &serial, Packet_t packet);
// =============== PACKET func: End ==================

// =============== Frame func: Begin ==================
bool Frame_ParseFromPacket(Packet_t packet, Frame_t *frame);
Packet_t Frame_ParseToPacket(Frame_t frame);
// =============== Frame func: End ==================

// =============== Support func: Begin ==================
void printHex(uint8_t value) {
    if (value < 0x10) Serial.print("0"); // Đảm bảo luôn có 2 ký tự HEX
    Serial.print(value, HEX);
}

void printPacketHex(uint8_t header, uint8_t* payload, uint8_t payloadLen) {
    printHex(header);
    Serial.print(" ");  // Dấu cách giữa các byte
    printHex(payloadLen);
    Serial.print(" ");
    
    for (uint8_t i = 0; i < payloadLen; i++) {
        printHex(payload[i]);
        Serial.print(" ");
    }
    
    Serial.println();  // Xuống dòng sau khi in xong
}
// =============== Support func: End ==================

SoftwareSerial LoRaSerial(LORA_RX_PIN, LORA_TX_PIN); 

void Lora_Init() {
    LoRaSerial.begin(9600);
}

void Lora_Send(Frame_t frame) {
    Packet_t packet = Frame_ParseToPacket(frame);
    if(packet.len <= 0) {
        return;
    }

    Packet_Send(LoRaSerial, packet);
}

bool Lora_Receive(Frame_t *frame) {
    Packet_t packet;

    if(!Packet_Read(LoRaSerial, &packet)) {
        return false;
    }

    Serial.print("RX:");
    printPacketHex(HEADER_BYTE, packet.payload, packet.len);

    return Frame_ParseFromPacket(packet, frame);
}


bool Frame_ParseFromPacket(Packet_t packet, Frame_t *frame)
{
    uint64_t devId = ((uint64_t)packet.payload[0] << 40) | ((uint64_t)packet.payload[1] << 32) | ((uint64_t)packet.payload[2] << 24) | ((uint64_t)packet.payload[3] << 16) | ((uint64_t)packet.payload[4] << 8) | (uint64_t)packet.payload[5];
    frame->devId = devId;

    uint8_t iPayload = 6;
    uint8_t iCommand = 0;
    while (iPayload < packet.len)
    {
        // get cmd
        frame->commands[iCommand].cmd = packet.payload[iPayload];
        frame->commands[iCommand].dataSize = Config_GetSizeByCmd(frame->commands[iCommand].cmd, RX_DIR);
        iPayload++;

        // valid dataSize
        if (frame->commands[iCommand].dataSize < 0 || (iPayload + frame->commands[iCommand].dataSize) > packet.len)
        {
            return false;
        }

        // copy value of cmd to command.data
        for (uint8_t i = 0; i < frame->commands[iCommand].dataSize; i++)
        {
            frame->commands[iCommand].data[i] = packet.payload[iPayload];
            iPayload++;
        }

        iCommand++;
    }
    frame->commandSize = iCommand;

    return true;
}


Packet_t Frame_ParseToPacket(Frame_t frame)
{
    Packet_t packet;
    uint8_t iPayload = 0;

    packet.payload[0] = (frame.devId >> 40) & 0xFF;
    packet.payload[1] = (frame.devId >> 32) & 0xFF;
    packet.payload[2] = (frame.devId >> 24) & 0xFF;
    packet.payload[3] = (frame.devId >> 16) & 0xFF;
    packet.payload[4] = (frame.devId >> 8) & 0xFF;
    packet.payload[5] = frame.devId & 0xFF;

    iPayload = 6;
    for (uint8_t i = 0; i < frame.commandSize; i++)
    {
        frame.commands[i].dataSize = Config_GetSizeByCmd(frame.commands[i].cmd, TX_DIR);
        if ((iPayload + 1 + frame.commands[i].dataSize) > MAX_PAYLOAD_SIZE)
        {
            return Packet_t{};
        }
        packet.payload[iPayload] = frame.commands[i].cmd;
        iPayload++;

        memcpy(&packet.payload[iPayload], frame.commands[i].data, frame.commands[i].dataSize);
        iPayload += frame.commands[i].dataSize;
    }

    packet.len = iPayload;

    return packet;
}


bool Packet_Read(SoftwareSerial &serial, Packet_t *result_packet)
{
    while (serial.available())
    {
        uint8_t data = serial.read();

        switch (rxState)
        {
        case RxHeader:
            if (data != HEADER_BYTE)
            {
                rxState = RxHeader;
            }
            else
            {
                rxState = RxLen;
            }
            break;

        case RxLen:
            g_packet.len = data;
            if (g_packet.len > MAX_PAYLOAD_SIZE || g_packet.len < MIN_PAYLOAD_SIZE)
            {
                rxState = RxHeader;
            }

            rxState = RxPayload;
            payload_cnt = 0;
            break;

        case RxPayload:
            g_packet.payload[payload_cnt] = data;
            payload_cnt++;
            if (payload_cnt >= g_packet.len)
            {
                // rx done
                rxState = RxHeader;
                *result_packet = g_packet;
                return true;
            }
            break;

        default:
            // never here
            break;
        }
    }

    return false;
}

void Packet_Send(SoftwareSerial &serial, Packet_t packet)
{
    if (packet.len > 0)
    {
        serial.write(HEADER_BYTE);
        serial.write(packet.len);
        serial.write(packet.payload, packet.len);

        Serial.print("TX:");
        printPacketHex(HEADER_BYTE, packet.payload, packet.len);
    }
}