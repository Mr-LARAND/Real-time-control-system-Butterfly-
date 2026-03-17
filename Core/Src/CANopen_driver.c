/*
 * CANopen_driver.c
 *
 *  Created on: 16 мар. 2026 г.
 *      Author: larand
 */

#include "CANopen_driver.h"
#include <string.h> // Для memcpy
#include <CANopen.h>

/* Глобальный объект CANopen */
static CO_t* CO_obj = NULL;
/* Указатель на CAN-модуль STM32 */
static CAN_HandleTypeDef* co_hcan;

/*
 * Эта функция вызывается изнутри CANopenNode для отправки CAN-кадра.
 * Мы должны реализовать ее, используя HAL.
 */
CO_ReturnError_t CO_CANsend(CO_CANmodule_t *CANmodule, const CO_CANtx_t *message) {
    if (HAL_GetTick() % 1000 < 5) { // Проверяем, что CAN-периферия готова
      if (HAL_CAN_GetTxMailboxesFreeLevel(co_hcan) == 0) {
          return CO_ERROR_TIMEOUT; // Нет свободных почтовых ящиков
      }
    }

    CAN_TxHeaderTypeDef TxHeader;
    TxHeader.StdId = message->ident;
    TxHeader.ExtId = 0;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = (message->ident & 0x08000000) ? CAN_RTR_REMOTE : CAN_RTR_DATA;
    TxHeader.DLC = message->DLC;
    TxHeader.TransmitGlobalTime = DISABLE;

    uint32_t TxMailbox; // Переменная для хранения номера почтового ящика

    if (HAL_CAN_AddTxMessage(co_hcan, &TxHeader, (uint8_t*)message->data, &TxMailbox) != HAL_OK) {
        // Ошибка отправки, можно добавить обработку
        return CO_ERROR_TIMEOUT;
    }
    return CO_ERROR_NO;
}


CO_ReturnError_t co_driver_init(CAN_HandleTypeDef* hcan, uint16_t bitRate, uint8_t nodeId) {
    co_hcan = hcan;

    // 1. Выделяем память для объекта CANopen.
    CO_obj = CO_new(NULL);
    if (CO_obj == NULL) {
        return CO_ERROR_OUT_OF_MEMORY;
    }

    // 2. Инициализируем CAN-модуль в стеке CANopen
    CO_ReturnError_t err = CO_CANinit(CO_obj, (uintptr_t)NULL, bitRate);
    if (err != CO_ERROR_NO) {
        return err;
    }

    // 3. Инициализируем сам узел (basic LSS, NMT, SDO, ...)
    err = CO_CANopen_basic_init(CO_obj, nodeId);
    if (err != CO_ERROR_NO) {
        return err;
    }

    // 4. Запускаем CAN-периферию STM32
    if (HAL_CAN_Start(co_hcan) != HAL_OK) {
        return CO_ERROR_HW;
    }

    // 5. Включаем прерывания на прием
    if (HAL_CAN_ActivateNotification(co_hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        return CO_ERROR_HW;
    }

    // 6. Устанавливаем CAN-узел в состояние "Pre-operational"
    CO_NMT_set_command(CO_obj->NMT, CO_NMT_ENTER_PRE_OPERATIONAL);

    return CO_ERROR_NO;
}

void co_driver_process(uint32_t time_diff_us) {
    if (CO_obj == NULL) return;

    // Обрабатываем входящие и исходящие CAN-сообщения
    CO_process(CO_obj, time_diff_us);
}

void co_driver_read_msg(CAN_RxHeaderTypeDef* RxHeader, uint8_t* RxData) {
    if (CO_obj == NULL) return;

    CO_CANrxMsg_t rxMsg;
    rxMsg.ident = RxHeader->StdId;
    rxMsg.DLC = RxHeader->DLC;
    memcpy(rxMsg.data, RxData, RxHeader->DLC);

    // Передаем принятое сообщение в стек CANopenNode для обработки
    CO_CANrx_receive(CO_obj->CAN, &rxMsg);
}

// Эта функция должна быть реализована, так как на нее ссылается стек.
// В нашем случае она не нужна, поэтому оставляем пустой.
void CO_errExit(char* str) {
    // Сюда можно добавить код для обработки критических ошибок,
    // например, мигание светодиодом или запись в лог.
    while(1);
}
