#include "sdo.h"
#include <string.h>

/*
 * Структура SDO-запроса (8 байт):
 * Byte 0       - Command Byte
 * Bytes 1-2    - Index
 * Byte 3       - Subindex
 * Bytes 4-7    - Data
 */
void SDO_Create_Read_Request(CAN_Frame *frame, uint8_t node_id, uint16_t index, uint8_t subindex)
{
    // 1. COB-ID для SDO-запроса 0x600 + node_id
    frame->id = 0x600 + node_id;
    frame->rtr = 0;

    // 2. SDO-запрос имеет 8 байт данных
    frame->dlc = 8;
    memset(frame->data, 0, frame->dlc);

    // 3. Формирование запроса
    frame->data[0] = 0x40;

    frame->data[1] = (uint8_t)(index & 0xFF);
    frame->data[2] = (uint8_t)((index >> 8) & 0xFF);

    frame->data[3] = subindex;
}

int SDO_Parse_Response(CAN_Frame *frame, uint8_t node_id, uint16_t *index, uint8_t *subindex, uint32_t *data, uint8_t *data_len)
{
    // Проверяем это SDO-Frame от нужного нам узла?
    if (frame->id != (0x580 + node_id) || frame->dlc != 8) 
        return 0;

    uint8_t command_byte = frame->data[0];

    // Проверка на ошибку
    if (command_byte == 0x80)
        return -1;

    *index = frame->data[1] | (frame->data[2] << 8);
    *subindex = frame->data[3];

    // Определяем длину данных по Command Specifier
    // В CANopen спецификации биты 2 и 3 в первом байте указывают на кол-во пустых байт
    if (command_byte == 0x43) 
        *data_len = 4;
    else if (command_byte == 0x4B) 
        *data_len = 2;
    else if (command_byte == 0x4F) 
        *data_len = 1;
    else
        // Если размер не указан явно для упрощения считаем, что там 4 байта
        *data_len = 4;

    memcpy(data, &frame->data[4], *data_len);
    return 1;  
}
