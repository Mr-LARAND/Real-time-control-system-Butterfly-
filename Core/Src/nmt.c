#include "nmt.h"

void NMT_Create_Command(CAN_Frame *frame, uint8_t command, uint8_t node_id)
{
    frame->id = 0x000; // COB-ID для NMT мастера равен 0x000.
    frame->dlc = 2;  // NMT сообщение всегда содержит ровно 2 байта данных

    frame->data[0] = command; // Какую команду выполняем
    frame->data[1] = node_id; // Кому предназначена команда (Какому узлу)

    frame->rtr = 0; // Передаём стандартный кадр (DATA-Frame)
}