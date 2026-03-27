#include "heartbeat.h"

int Heartbeat_Parse(const CAN_Frame *frame, uint8_t *node_id, uint8_t *state) 
{
    // COB-ID для Heartbeat = 0x700 + Node ID
    // Проверяем, находится ли ID кадра в этом диапазоне.
    if (frame->id >= 0x701 && frame->id <= 0x77F) {
        // Heartbeat всегда содержит 1 байт данных (состояние)
        if (frame->dlc >= 1) {
            *state = frame->data[0];
            *node_id = frame->id & 0x7F; // Младшие 7 бит ID - это ID узла
            return 1;
        }
    }
    return 0;
}

const char* NMT_Print_State(uint8_t state) 
{
    switch(state) 
    {
        case NMT_STATE_BOOTUP:          return "Boot-Up";
        case NMT_STATE_STOPPED:         return "Stopped";
        case NMT_STATE_OPERATIONAL:     return "Operational";
        case NMT_STATE_PRE_OPERATIONAL: return "Pre-Operational";
        default:                        return "Unknown";
    }
}