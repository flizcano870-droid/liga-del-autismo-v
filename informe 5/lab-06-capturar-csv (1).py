#!/usr/bin/env python3
"""
Lab 6 — Script de Captura CSV
Electrónica Digital 2026-1 — Universidad Nacional de Colombia

Captura el flujo CSV emitido por el Arduino (3 sensores: LDR, LM35, potenciómetro)
y lo escribe a un archivo .csv en disco.

Uso:
  1. Cerrar el Serial Monitor del Arduino IDE (el puerto solo acepta un cliente)
  2. Ejecutar: python3 capturar_csv.py
  3. Estimular los sensores durante la captura (~60 s)
  4. Presionar Ctrl+C para detener y cerrar limpiamente

Requisitos:
  pip install pyserial
"""

import serial
import sys
from datetime import datetime


def detectar_puerto():
    """
    Intenta detectar automáticamente el puerto del Arduino.
    Si no lo encuentra, muestra los puertos disponibles y pide al usuario.
    """
    import serial.tools.list_ports

    puertos = list(serial.tools.list_ports.comports())
    if not puertos:
        print("ERROR: No se detectaron puertos seriales.")
        print("¿Está el Arduino conectado por USB?")
        sys.exit(1)

    # Si solo hay un puerto, usarlo directamente
    if len(puertos) == 1:
        return puertos[0].device

    # Mostrar opciones
    print("Puertos seriales detectados:")
    for i, p in enumerate(puertos):
        print(f"  [{i}] {p.device} — {p.description}")

    seleccion = input(f"Seleccione el puerto [0-{len(puertos)-1}]: ").strip()
    try:
        idx = int(seleccion)
        return puertos[idx].device
    except (ValueError, IndexError):
        print("Selección inválida. Abortando.")
        sys.exit(1)


def main():
    puerto = detectar_puerto()
    baudrate = 9600

    # Generar nombre de archivo con timestamp
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    nombre_archivo = f"datos_s6_{timestamp}.csv"

    print(f"Conectando a {puerto} a {baudrate} baudios...")
    print(f"Archivo de salida: {nombre_archivo}")
    print("Estimule los sensores durante la captura.")
    print("Presione Ctrl+C para detener.\n")

    try:
        ser = serial.Serial(puerto, baudrate, timeout=1)

        # Descartar datos residuales en el buffer
        ser.reset_input_buffer()

        with open(nombre_archivo, 'w') as archivo:
            filas = 0
            while True:
                linea = ser.readline().decode('utf-8', errors='replace').strip()

                if linea:
                    print(linea)
                    archivo.write(linea + '\n')
                    archivo.flush()  # Forzar escritura a disco
                    filas += 1

    except KeyboardInterrupt:
        print(f"\n\nCaptura detenida. {filas} filas guardadas en '{nombre_archivo}'.")
        print(f"Siguiente paso: abrir '{nombre_archivo}' en Excel/Google Sheets.")
    except serial.SerialException as e:
        print(f"\nERROR de puerto serial: {e}")
        print("¿Cerraste el Serial Monitor del Arduino IDE antes de ejecutar este script?")
        sys.exit(1)
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Puerto serial cerrado.")


if __name__ == '__main__':
    main()