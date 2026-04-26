#!/usr/bin/env python3
"""
terminal_crudo.py
Laboratorio 4 — Parte 4, Actividad 7: Terminal de comunicación directa

PROPÓSITO PEDAGÓGICO:
    Este script es un terminal mínimo que envía exactamente lo que
    el estudiante escribe al Arduino y muestra la respuesta cruda.
    El estudiante escribe los comandos de formato fijo a mano
    ("ST 00000", "ON 00000", etc.) y ve las respuestas sin ningún
    procesamiento de por medio.

    Es incómodo escribir el formato fijo manualmente — eso es intencional.
    En la Actividad 8 se construye un cliente con menú que lo hace
    automáticamente.

INSTRUCCIONES:
    1. Cierra el Monitor Serial del Arduino IDE antes de ejecutar.
       (Solo un programa puede usar el puerto serial a la vez.)
    2. Verifica que el puerto en la variable PUERTO_SERIAL coincida
       con el que aparece en Arduino IDE → Herramientas → Puerto.
    3. Ejecuta: python3 terminal_crudo.py
    4. Escribe comandos en formato "CC NNNNN" y presiona Enter.
    5. Escribe "salir" para cerrar.

PUERTOS COMUNES:
    Linux:   /dev/ttyACM0  o  /dev/ttyUSB0
    macOS:   /dev/cu.usbmodem...
    Windows: COM3  (o el número que asigne tu sistema)
"""

import serial  # pip install pyserial


# =============================================================
# CONFIGURACIÓN — ajusta PUERTO_SERIAL si es necesario
# =============================================================
PUERTO_SERIAL = "/dev/ttyACM0"   # <-- verificar y corregir si es diferente
BAUDRATE      = 9600
TIMEOUT_S     = 2                # Tiempo máximo de espera para respuesta


# =============================================================
# PROGRAMA PRINCIPAL
# =============================================================
def main():
    # Abrir el puerto serial
    try:
        puerto = serial.Serial(PUERTO_SERIAL, BAUDRATE, timeout=TIMEOUT_S)
    except serial.SerialException as e:
        print(f"Error: no se pudo abrir {PUERTO_SERIAL}")
        print(f"Detalle: {e}")
        print("Verifica que el Arduino esté conectado y que el Monitor Serial esté cerrado.")
        return

    print(f"Conectado a {PUERTO_SERIAL} a {BAUDRATE} baudios.")
    print("Escribe comandos en formato 'CC NNNNN' (ej: ST 00000, BL 00005).")
    print("Escribe 'salir' para terminar.\n")

    # Leer y descartar el mensaje de bienvenida del Arduino
    import time
    time.sleep(2)
    while puerto.in_waiting > 0:
        linea = puerto.readline().decode("utf-8", errors="replace").strip()
        print(f"Arduino: {linea}")

    # Bucle principal: pedir comando, enviar, mostrar respuesta
    while True:
        comando = input("\nTX > ").strip()

        if comando.lower() == "salir":
            break

        # Enviar al Arduino (agregar \n como delimitador de trama)
        puerto.write((comando + "\n").encode("utf-8"))

        # Leer respuesta (readline espera hasta \n o hasta TIMEOUT_S)
        respuesta = puerto.readline().decode("utf-8", errors="replace").strip()
        print(f"RX < {respuesta}")

    puerto.close()
    print("Puerto cerrado. Hasta luego.")


if __name__ == "__main__":
    main()