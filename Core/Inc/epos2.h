/*
 * epos2.h
 *
 *  Created on: 28 мар. 2026 г.
 *      Author: larand
 */

#ifndef INC_EPOS2_H_
#define INC_EPOS2_H_

#include <stdint.h>

/*
 * Инициализирует драйвер EPOS2, переводя его в рабочее состояние.
 * return: 0 при успехе, -1 при ошибке
 */
int EPOS2_Init(uint8_t node_id);

#endif /* INC_EPOS2_H_ */
