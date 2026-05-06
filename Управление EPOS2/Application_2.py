"""
'SPEED' - Задать скорость
'STOP' - Отключить силовой элемент
'START' - Включить силовой элемент
'HELP' - Вывксти справку по командам
"""

import socket
import struct
import threading
import time

UDP_IP = "192.168.1.10"
UDP_PORT = 5000

# ID Команд
CMD_SET_VELOCITY = 0x01
CMD_GET_TELEMETRY = 0x02
CMD_STOP = 0x03
CMD_START = 0x04
RESP_TELEMETRY = 0x12

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
# Привязываем сокет локально, чтобы можно было слушать ответы асинхронно
sock.bind(("", 5000))
sock.settimeout(0.5)

# Флаг для остановки потока
running = True


# --- ПОТОК ДЛЯ ПРИЕМА ТЕЛЕМЕТРИИ ---
def receive_telemetry():
    while running:
        try:
            data, addr = sock.recvfrom(1024)
            if len(data) >= 1:
                cmd_id = data[0]
                if cmd_id == RESP_TELEMETRY and len(data) == 13:
                    # Распаковываем: < (Little Endian), b (1 byte), i i i (три 4-байтных int)
                    unpacked = struct.unpack("<biii", data)
                    pos = unpacked[1]
                    vel = unpacked[2]
                    torq = unpacked[3]
                    print(f"\n[ТЕЛЕМЕТРИЯ] Позиция: {pos} | Скорость: {vel} | Момент: {torq}")
                    print("Ожидание ввода: ", end="", flush=True)  # Восстанавливаем приглашение
        except socket.timeout:
            pass  # Это нормально, просто ждем дальше
        except Exception as e:
            if running: print(f"\nОшибка приема: {e}")


# Запускаем прием в отдельном фоновом потоке
recv_thread = threading.Thread(target=receive_telemetry)
recv_thread.start()

# --- ОСНОВНОЙ ПОТОК (ОТПРАВКА КОМАНД) ---
print(f"Подключение к {UDP_IP}:{UDP_PORT}")
print("Команды: SPEED, STOP, START, GET.  Для выхода: Ctrl+C\n")

try:
    while True:
        str_cmd = input("Ожидание ввода: ").strip().upper()

        if str_cmd == "SPEED":
            speed_str = input("Введите скорость (RPM): ").strip()
            try:
                speed_int = int(speed_str)
                # Упаковываем: ID (0x01) и Скорость (int32). Формат "<bi"
                packet = struct.pack("<bi", CMD_SET_VELOCITY, speed_int)
                sock.sendto(packet, (UDP_IP, UDP_PORT))
            except ValueError:
                print("Ошибка: Введите целое число!")

        elif str_cmd == "STOP":
            packet = struct.pack("<b", CMD_STOP)
            sock.sendto(packet, (UDP_IP, UDP_PORT))

        elif str_cmd == "START":
            packet = struct.pack("<b", CMD_START)
            sock.sendto(packet, (UDP_IP, UDP_PORT))

        elif str_cmd == "GET":
            # Запрашиваем телеметрию
            packet = struct.pack("<b", CMD_GET_TELEMETRY)
            sock.sendto(packet, (UDP_IP, UDP_PORT))

        elif str_cmd != "":
            print("Неизвестная команда.")

except KeyboardInterrupt:
    running = False  # Останавливаем фоновый поток
    # Отправляем STOP
    packet = struct.pack("<b", CMD_STOP)
    sock.sendto(packet, (UDP_IP, UDP_PORT))
    print("\nВыход.")

recv_thread.join()
sock.close()
