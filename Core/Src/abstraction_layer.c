#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stm32f7xx_hal.h>

#include "abstraction_layer.h"

CAN_HandleTypeDef hcan1; // !!! Возможно нужно будет добавить extern

/* !!! УБРАТЬ У ФУНКЦИЙ ПАРАМЕТРЫ (Если будет время :) )!!!*/

int can_init(const char *ifname) 
{
	// Настройка фильтра на приём всех сообщений
    CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAK_OK)
    	return -1;

    if (HAL_CAN_Start(&hcan1) != HAL_OK)
    	return -1;

    return 0;
}

int can_send(int sock_fd, const CAN_Frame *frame) 
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;
    
    // Конвертируем универсальный кадр в структуру HAL STM32
    TxHeader.StdId = frame->id;
    TxHeader.ExtId = 0x00;
    TxHeader.IDE = CAN_ID_STD; // CANopen использует стандартные 11-битные ID
    TxHeader.RTR = frame->rtr ? CAN_RTR_REMOTE : CAN_RTR_DATA; // Скорее всего мы будем передавать Data Frame, но пусть будет проверка
    TxHeader.DLC = frame->dlc;
    TxHeader.TransmitGlobalTime = DISABLE;

    // Отправка в шину
    if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, (uint8_t *)frame->data, &TxMailbox) != HAL_OK)
     	return -1;

    return 0;
}

int can_receive(int sock_fd, CAN_Frame *frame) 
{
	CAN_RxHeaderTypeDef RxHeader;

	// Проверка есть ли сообщения в буфере FIFO0
	if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) > 0)
	{
		if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, frame->data) == HAL_OK) {
			frame->id = RxHeader.StdId;
			frame->dlc = RxHeader.DLC;
			frame->rtr = (RxHeader.RTR == CAN_RTR_REMOTE) ? 1 : 0;

			return 1;
		}
	}
	return 0; // Если кадров нет
}

void can_close(int sock_fd) {
	HAL_CAN_Stop(&hcan1);
}
