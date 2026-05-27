# =========================================================
# LAB 09 - GUARDAR CADA INTENTO EN UN CSV DISTINTO
# =========================================================

import serial
from datetime import datetime

# =========================================================
# CONFIGURACION
# =========================================================

PUERTO = 'COM6'     # CAMBIAR
BAUDIOS = 115200

# =========================================================
# CREAR NOMBRE AUTOMATICO
# =========================================================

tiempo = datetime.now().strftime("%Y%m%d_%H%M%S")

nombreArchivo = f"medicion_{tiempo}.csv"

# =========================================================
# SERIAL
# =========================================================

ser = serial.Serial(PUERTO, BAUDIOS)

print("Puerto abierto")

print("Archivo:", nombreArchivo)

# =========================================================
# ARCHIVO CSV
# =========================================================

archivo = open(nombreArchivo, "w")

# =========================================================
# LOOP
# =========================================================

while True:

    try:

        linea = ser.readline().decode().strip()

        print(linea)

        archivo.write(linea + "\n")

        archivo.flush()

    except KeyboardInterrupt:

        print("\nCaptura finalizada")

        archivo.close()

        ser.close()

        break