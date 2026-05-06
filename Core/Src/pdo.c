#include "pdo.h"
#include <string.h>

void PDO_Create_RxPDO(CAN_Frame *frame, uint8_t pdo_num, uint8_t node_id, const uint8_t *data, uint8_t data_len)
{
    if (pdo_num >= 4 || pdo_num < 1)
        return; // Неверный номер PDO

    uint16_t cob_id_base = 0;
    // Определение COB-ID в зависимости от номера PDO
    switch (pdo_num)
    {
    case 1: cob_id_base = 0x200; break;
    case 2: cob_id_base = 0x300; break;
    case 3: cob_id_base = 0x400; break;
    case 4: cob_id_base = 0x500; break;
    }

    frame->id = cob_id_base + node_id;
    frame->rtr = 0;

    // Защита от переполнения
    frame->dlc = (data_len > 8) ? 8 : data_len;

    memcpy(frame->data, data, frame->dlc);
}

int PDO_Parse_TxPDO(const CAN_Frame *frame, uint8_t *pdo_num, uint8_t *node_id, uint8_t *data, uint8_t *data_len)
{
    // Проверяем диапазон TxPDO 1 (0x181 - 0x1FF)
    if (frame->id >= 0x181 && frame->id <= 0x1FF) {
        *node_id = frame->id - 0x180;
        *pdo_num = 1;
    }
    // Проверяем диапазон TxPDO 2 (0x281 - 0x2FF)
    else if (frame->id >= 0x281 && frame->id <= 0x2FF) {
        *node_id = frame->id - 0x280;
        *pdo_num = 2;
    }
    // Проверяем диапазон TxPDO 3 (0x381 - 0x3FF)
    else if (frame->id >= 0x381 && frame->id <= 0x3FF) {
        *node_id = frame->id - 0x380;
        *pdo_num = 3;
    }
    // Проверяем диапазон TxPDO 4 (0x481 - 0x4FF)
    else if (frame->id >= 0x481 && frame->id <= 0x4FF) {
        *node_id = frame->id - 0x480;
        *pdo_num = 4;
    }
    else {
        return 0; // Это не TxPDO-Frame
    }
    
    *data_len = frame->dlc;
    memcpy(data, frame->data, *data_len);

    return 1;
}
