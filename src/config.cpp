#include "config.h"

typedef struct {
    int8_t tx_dir;
    int8_t rx_dir;
} SizeCmdByDir_t;

static SizeCmdByDir_t cmdLenTable[256];

void Config_Init()
{
    memset(cmdLenTable, -1, sizeof(cmdLenTable));

    // ================ Table {cmd, data size} ==================
    cmdLenTable[TemperatureReport] = {1, 0};
    cmdLenTable[HumidityReport] = {1, 0};
    cmdLenTable[SoilReport] = {1, 0};
    cmdLenTable[LuxReport] = {2, 0};
    cmdLenTable[Relay1Report] = {1, 0};
    cmdLenTable[Relay2Report] = {1, 0};

    cmdLenTable[Relay1Get] = {1, 0};
    cmdLenTable[Relay1Set] = {1, 1};

    cmdLenTable[Relay2Get] = {1, 0};
    cmdLenTable[Relay2Set] = {1, 1};
}

int8_t Config_GetSizeByCmd(uint8_t cmd, DataDirection_t dir)
{
    if(dir == TX_DIR) {
        return cmdLenTable[cmd].tx_dir;
    }

    return cmdLenTable[cmd].rx_dir;
}