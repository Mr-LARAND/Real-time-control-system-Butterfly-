/*
 * CANopen_driver.h
 *
 *  Created on: 16 мар. 2026 г.
 *      Author: larand
 */

#ifndef SRC_CANOPEN_DRIVER_H_
#define SRC_CANOPEN_DRIVER_H_

#include "CANopen.h"
#include "stm32f7xx_hal.h" // Замените на ваш заголовочный файл HAL, если он другой

/*
 * Инициализирует CAN-модуль и связывает его со стеком CANopenNode
 * @param hcan Указатель на структуру CAN_HandleTypeDef от STM32 HAL
 * @param bitRate Скорость шины в кбит/с (например, 500)
 * @param nodeId ID этого узла (вашего STM32)
 * @return CO_ReturnError_t, CO_ERROR_NO в случае успеха
 */
CO_ReturnError_t co_driver_init(CAN_HandleTypeDef* hcan, uint16_t bitRate, uint8_t nodeId);

/*
 * Обрабатывает основной цикл CANopen. Эту функцию нужно вызывать в главном цикле while(1).
 * @param time_diff_us Разница во времени с прошлого вызова в микросекундах
 */
void co_driver_process(uint32_t time_diff_us);

/*
 * Функция-обертка для приема CAN-сообщения. Вызывается из прерывания.
 * @param RxHeader Указатель на заголовок полученного сообщения
 * @param RxData Указатель на данные полученного сообщения
 */
void co_driver_read_msg(CAN_RxHeaderTypeDef* RxHeader, uint8_t* RxData);

#endif /* SRC_CANOPEN_DRIVER_H_ */
