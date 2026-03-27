#ifndef ABSTRACTION_LAYER_H
#define ABSTRACTION_LAYER_H

#include "canopen_types.h"

/*
 * Инициализирует CAN-интерфейс (в нашем случае, SocketCAN)
 * ifname - имя интерфейса, например "can0"
 * Возвращает: файловый дескриптор сокета или -1 в случае ошибки.
 */
int can_init(const char *ifname);

/*
 * Отправляет CAN-кадр через заданный сокет.
 * sock_fd - дескриптор, полученный от can_init()
 * frame - указатель на наш универсальный CAN-кадр
 * Возвращает: 0 при успехе, -1 при ошибке.
 */
int can_send(int sock_fd, const CAN_Frame *frame); // Изменить название на can_transmit

/*
 * Принимает CAN-кадр из сокета.
 * sock_fd - дескриптор, полученный от can_init()
 * frame - указатель на структуру, куда будут записаны данные
 * Возвращает: 1 если кадр получен, 0 если кадров нет, -1 при ошибке.
 */
int can_receive(int sock_fd, CAN_Frame *frame);

/*
 * Закрывает CAN-сокет.
 */
void can_close(int sock_fd);

#endif
