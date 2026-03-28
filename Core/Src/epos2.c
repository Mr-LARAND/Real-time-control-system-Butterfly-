/*
 * epos2.c
 *
 *  Created on: 28 мар. 2026 г.
 *      Author: larand
 */

#include "epos2.h"
#include "canopen_types.h"
#include "abstraction_layer.h"
#include "sdo.h"
#include "pdo.h"
#include "nmt.h"
#include "stm32f7xx_hal.h" // Для HAL_Delay

// Вспомогательная функция для SDO-записи (аналог SDO_CreateReadRequest)
static void SDO_CreateWriteRequest(CAN_Frame *frame, uint8_t node_id, uint16_t index, uint8_t subindex, uint32_t data, uint8_t data_len)
{
    frame->id = 0x600 + node_id;
    frame->rtr = 0;
    frame->dlc = 8;

    // Command Specifier для записи зависит от длины данных
    // 0x23 (4 байта), 0x2B (2 байта), 0x2F (1 байт)
    switch(data_len) {
        case 1: frame->data[0] = 0x2F; break;
        case 2: frame->data[0] = 0x2B; break;
        case 4: frame->data[0] = 0x23; break;
        default: return; // Не поддерживаем другую длину для примера
    }

    // Индекс и субиндекс (little-endian)
    frame->data[1] = (uint8_t)(index);
    frame->data[2] = (uint8_t)(index >> 8);
    frame->data[3] = subindex;

    // Данные (little-endian)
    frame->data[4] = (uint8_t)(data);
    frame->data[5] = (uint8_t)(data >> 8);
    frame->data[6] = (uint8_t)(data >> 16);
    frame->data[7] = (uint8_t)(data >> 24);
}

// Главная функция инициализации
int EPOS2_Init(uint8_t node_id)
{
    CAN_Frame frame;

    // ШАГ 1: Останавливаем узел, чтобы гарантированно перейти в Pre-Operational
    NMT_Create_Command(&frame, NMT_PRE_OPERATIONAL, node_id);
    can_send(0, &frame);
    HAL_Delay(100); // Даем время на переход

    // ШАГ 2: Настраиваем основные параметры через SDO

    // Пример: Устанавливаем режим работы "Profile Velocity Mode" (-1)
    // Индекс 0x6060, субиндекс 0x00, данные = 3 (тип SINT8, но пишем как 1 байт)
    SDO_CreateWriteRequest(&frame, node_id, 0x6060, 0, 3, 1);
    can_send(0, &frame);
    HAL_Delay(50); // Пауза между SDO

    // Пример: Устанавливаем максимальную скорость профиля (например, 3000 об/мин)
    // Индекс 0x6081, субиндекс 0x00, данные = 3000 (тип UINT32)
    SDO_CreateWriteRequest(&frame, node_id, 0x6081, 0, 3000, 4);
    can_send(0, &frame);
    HAL_Delay(50);

    // ШАГ 3: Переводим привод в состояние "Ready to Switch On"
    // Это делается отправкой Controlword = 0x0006 через PDO
    uint8_t pdo_data_shutdown[2] = {0x06, 0x00}; // 0x0006 в little-endian
    PDO_Create_RxPDO(&frame, 1, node_id, pdo_data_shutdown, 2);
    can_send(0, &frame);
    HAL_Delay(50);

    // ШАГ 4: Включаем привод (состояние "Switched On")
    // Controlword = 0x000F
    uint8_t pdo_data_switch_on[2] = {0x0F, 0x00};
    PDO_Create_RxPDO(&frame, 1, node_id, pdo_data_switch_on, 2);
    can_send(0, &frame);
    HAL_Delay(50);

    //ШАГ 5: Разрешаем работу (состояние "Operation Enabled")
    // Controlword остается 0x000F, но бит "Enable Operation" тоже устанавливается.
    // На практике, после 0x000F драйвер сам переходит в это состояние.

    // ШАГ 6: Переводим всю сеть в Operational, чтобы PDO работали ---
    NMT_Create_Command(&frame, NMT_OPERATIONAL, node_id);
    can_send(0, &frame);
    HAL_Delay(100);

    // Здесь можно добавить проверку Statusword, чтобы убедиться, что всё прошло успешно.
    // Если Statusword == 0x0037, значит, мы в "Operation Enabled".

    return 0; // Инициализация завершена
}
