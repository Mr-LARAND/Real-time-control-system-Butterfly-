#ifndef SDO_H
#define SDO_H

#include "canopen_types.h"

/*
 * Функция для формирования Expedited SDO-запроса на чтение.
 * frame    - указатель на кадр, который мы заполним
 * node_id  - ID узла (1 - 127), у которого мы запрашиваем данные
 * index    - 16-битный индекс объекта в OD
 * subindex - 8-битный субиндекс объекта в OD
 */
void SDO_Create_Read_Request(CAN_Frame *frame, uint8_t node_id, uint16_t index, uint8_t subindex);

/*
 * Функция для парсинга ответа на Expedited SDO-запрос.
 * frame - указатель на принятый кадр
 * node_id  - ID узла от которого мы ждём ответ
 * index    - указатель куда запишется прочитанный индекс
 * subindex - указатель куда запишется прочитанный субиндекс
 * data     - указатель на буфер, куда запишуться данные
 * data_len - указатель куда запишеться длинна данных
 * 
 * return: 1 при успешнои парсинге, 0 если это не SDO-Frame и -1 если произошла ошибка
 */
int SDO_Parse_Response(CAN_Frame *frame, uint8_t node_id, uint16_t *index, uint8_t *subindex, uint32_t *data, uint8_t *data_len);

#endif