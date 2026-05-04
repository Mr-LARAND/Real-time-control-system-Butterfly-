"""
'SPEED' - Задать скорость
'STOP' - Отключить силовой элемент
'START' - Включить силовой элемент
'HELP' - Вывксти справку по командам
"""

import socket

UDP_IP = "192.168.1.10" # Адерс STM32F767ZI
UDP_PORT = 5000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

print(f"Maxon Motor. Подключение к {UDP_IP}:{UDP_PORT}")
print("Введите 'HELP' для справки. Для выхода нажмите Ctrl+C")


try:
    while True:
        str_cmd = input("Ожидание ввода: ").strip().upper()

        if str_cmd == "SPEED":
            speed_str = input("Введите скорость (RPM): ").strip()
            try:
                speed_int = int(speed_str)
                # Отправляем строку в кодировке UTF-8
                sock.sendto(speed_str.encode('utf-8'), (UDP_IP, UDP_PORT))
                print(f"Отправлена команда: {speed_int} RPM")
            except ValueError:
                print("Ошибка: Введите целое число!")

        elif str_cmd == "STOP":
            sock.sendto(b"CMD_STOP", (UDP_IP, UDP_PORT))
            print("Силовой каскад выключен!\n")

        elif str_cmd == "START":
            sock.sendto(b"CMD_START", (UDP_IP, UDP_PORT))
            print("Силовой каскад включен!\n")

        elif str_cmd == "HELP":
            print("\nСПРАВКА ПО КОМАНДАМ\n")
            print(" SPEED  - Задать скорость мотора (потребует ввод числа)")
            print(" STOP   - Отключить силовой каскад (вал станет свободным, Controlword = 0x07)")
            print(" START  - Включить силовой каскад (вал станет упругим, Controlword = 0x0F)")
            print(" HELP   - Вывести это меню\n")
        elif str_cmd == "":
            pass
        else:
            print("Введена некорректная команда! Введите HELP для справки.\n")

except KeyboardInterrupt:
    sock.sendto(b"0", (UDP_IP, UDP_PORT))
    sock.sendto(b"CMD_STOP", (UDP_IP, UDP_PORT))
    print("\nМотор остановлен. Выход.")

