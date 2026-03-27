#ifndef PDO_H
#define PDO_H

#include "canopen_types.h"

/*
 * Формирует кадр RxPDO (команду от потребителя к производителю).
 * frame     - указатель на кадр для заполнения
 * pdo_num   - номер PDO (от 1 до 4)
 * node_id   - ID целевого узла (1-127)
 * data      - массив "сырых" данных (например, Controlword + Target Velocity)
 * data_len  - длина данных (от 1 до 8 байт)
 */
void PDO_Create_RxPDO(CAN_Frame *frame, uint8_t pdo_num, uint8_t node_id, const uint8_t *data, uint8_t data_len);

/*
 * Парсит кадр TxPDO (состояние от производителя к потребителю).
 * frame    - принятый CAN-кадр
 * pdo_num  - указатель, куда запишется номер PDO (1-4)
 * node_id  - указатель, куда запишется ID узла-отправителя
 * data     - буфер, куда скопируются данные
 * data_len - указатель, куда запишется длина полученных данных
 * 
 * return: 1 если это кадр TPDO, 0 если другой кадр.
 */
int PDO_Parse_TxPDO(const CAN_Frame *frame, uint8_t *pdo_num, uint8_t *node_id, uint8_t *data, uint8_t *data_len);

#endif
