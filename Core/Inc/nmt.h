#ifndef NMT_H
#define NMT_H

#include "canopen_types.h"

#define NMT_OPERATIONAL             0x01 // Перевод узла в Operational (Запуск)
#define NMT_STOPPED                 0x02 // Перевод узла в Stopped (Остановка)
#define NMT_PRE_OPERATIONAL         0x80 // Перевод узла в Pre-Operational (Настройка) 
#define NMT_RESET_APPLICATION       0x81 // Полная перезагрузка узла
#define NMT_RESET_COMMUNICATION     0x82 // Перезагрузка только связи CAN

/* 
 * Функция для формирования CANopen-Frame с командой NMT.
 * frame    - указатель на пустой кадр, который мы заполним
 * command  - одна из команд NMT_OPERATIONAL, NMT_STOPPED, NMT_PRE_OPERATIONAL...
 * node_id  - ID узла (от 1 до 127). Если передать 0, команда уйдет всем узлам
 */
void NMT_Create_Command(CAN_Frame *frame, uint8_t command, uint8_t node_id);

#endif