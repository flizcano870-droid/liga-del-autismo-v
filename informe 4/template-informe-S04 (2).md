# Informe de Laboratorio — Sesión 5: Control PWM y Actuadores

---

**Universidad Nacional de Colombia**
**Electrónica Digital — 2016684 — 2026-1**
**Prof. Ricardo Amézquita Orozco**

---

| Campo | |
|-------|--|
| **Integrantes** | 1. Felipe Lizcano Quimbaya|
| | 2. Sergio Andrés Poveda Pérez|
| | 3. Simón Gabriel Snadoval Palma|
| | 4. Sara Romero Chaves|
| | 5. |
| **Grupo** | 3 |
| **Fecha de la práctica** |29/04/2026 |
| **Fecha de entrega** | **Martes 22 de abril de 2026, 23:59** |

---

## 1. Resultados


### Actividad 1 — PWM con LED: Captura Serial Monitor

**Figura 1 — Serial Monitor (Actividad 1)**

Adjuntar captura de pantalla del Serial Monitor mostrando **al menos 10 filas** de las tres columnas correspondientes a distintas posiciones del potenciómetro.

**Caption obligatorio:** Indicar el valor de duty cycle al 0 %, al ~50 % y al 100 %.


![Figura 1 — Serial Monitor (Actividad 1)](Actividad1.png)

> **Descripción:** _(Describir brevemente qué muestra la captura y confirmar la relación lineal entre ADC y duty cycle observada.)_

---

### Actividad 3 — Control UART: Tabla del Protocolo Diseñado

**Tabla 1 — Protocolo UART Diseñado por el Grupo**


| Código | Descripción                         | Parámetro               | Respuesta del Arduino        |
|--------|-------------------------------------|--------------------------|------------------------------|
| VM N   | Cambia la velocidad del motor (PWM) | N: entero entre 0 y 255  | OK VEL N                     |
| DI N   | Cambia la dirección del motor       | N: 0 = CW, 1 = CCW       | OK DIR CW / OK DIR CCW       |
| ES     | Consulta el estado actual del motor | Sin parámetro            | EST V=<vel> D=CW / D=CCW     |
_Completar con los comandos implementados por el grupo. Mínimo tres filas._

---

### Actividad 3 — Control UART: Captura Serial Monitor

**Figura 2 — Serial Monitor (Actividad 3): Intercambios UART**

Adjuntar captura de pantalla del Serial Monitor mostrando **al menos tres intercambios comando→respuesta**, incluyendo un comando de velocidad, uno de dirección y uno de estado.

**Caption obligatorio:** Indicar los códigos de comando usados por el grupo.


![Figura 2— Serial Monitor (Actividad 3)](actividad3.jpeg)


> **Descripción:** _(Describir los comandos enviados y confirmar que el motor respondió sin necesidad de recompilar.)_

---

### Actividad 4 — Encoder: Tabla Duty Cycle vs RPM

**Tabla 2 — Caracterización Duty Cycle vs RPM**


| Duty Cycle (0–255) | RPM medidas |
|:-------------------:|:-----------:|
| 0 | |
| 25 | |
| 50 | |
| 75 | |
| 100 | |
| 125 | |
| 150 | |
| 175 | |
| 200 | |
| 225 | |
| 255 | |
| **Duty cycle mínimo de arranque:** | |

---

## 2. Visualización


### Figura 3 — Curva Duty Cycle vs RPM (Actividad 4)

**Eje X:** Duty Cycle (0–255)
**Eje Y:** RPM medidas

**Requisitos de la gráfica:**
- Marcar claramente la **zona muerta** (RPM = 0 para duty cycle bajo).
- Marcar el punto de **duty cycle mínimo de arranque**.
- Marcar **dos puntos representativos de la zona lineal** para calcular la pendiente (RPM por unidad de duty cycle). Etiquetar ambos puntos con sus coordenadas.
- Indicar si la zona lineal es aproximadamente proporcional o presenta saturación.

![Figura 3 — Curva Duty Cycle vs RPM](imagenes/figura-3-curva-pwm-rpm.png)

> **Interpretación:** _(Describir la forma de la curva: zona muerta, punto de arranque, comportamiento en zona lineal. Indicar si la relación es aproximadamente proporcional. Incluir el valor calculado de la pendiente en la zona lineal.)_

---

## 3. Análisis


### Preguntas de Análisis — Actividad 1

**Pregunta A1.1:** ¿Cuántos niveles de brillo distintos puede producir `analogWrite()`?

> [Respuesta del estudiante aquí]

**Pregunta A1.2:** ¿Qué ocurre con el brillo cuando el duty cycle es 0? ¿Y cuando es 255?

> [Respuesta del estudiante aquí]

**Pregunta A1.3:** ¿La frecuencia medida con el osciloscopio coincide con los ~490 Hz documentados para el Timer 1?

> [Respuesta del estudiante aquí]

**Pregunta A1.4:** La función `analogWrite()` genera una señal de frecuencia fija (~490 Hz en D9) con duty cycle variable. ¿Por qué el LED responde con brillo proporcional al duty cycle en lugar de parpadear a 490 Hz? ¿A partir de qué frecuencia mínima aproximada deja de percibirse el parpadeo en el ojo humano?

> [Respuesta del estudiante aquí]

---

### Pregunta de Análisis — Actividad 2

**Pregunta A2.1:** ¿Qué limitación tiene el método de control con velocidad hardcodeada? ¿Qué se debe hacer cada vez que se quiere cambiar la velocidad?

> [Respuesta del estudiante aquí]

---

### Pregunta de Análisis — Actividad 3

**Pregunta A3.1:** En el protocolo UART, el parser identifica los comandos comparando `buf[0]` y `buf[1]` directamente en lugar de usar `strcmp()`. ¿Qué condición del formato `CC N\n` hace que esta estrategia sea suficiente? ¿Seguiría siendo válida si el protocolo usara comandos de longitud variable (como en la Parte 2 de S4)?

> [Respuesta del estudiante aquí]

---

### Preguntas de Análisis — Actividad 4

**Pregunta A4.1:** Con base en la Tabla 2: ¿a qué duty cycle arranca el motor por primera vez? ¿Qué porcentaje de la escala total (0–255) representa esa zona muerta?

> [Respuesta del estudiante aquí]

**Pregunta A4.2:** La curva duty cycle vs RPM muestra una zona muerta en valores bajos de duty cycle. ¿Qué fenómeno físico del motor DC explica esa zona? ¿Cómo afectaría la existencia de esa zona a un sistema de control PID que intentara regular la velocidad del motor?

> [Respuesta del estudiante aquí]

---

### Preguntas de Análisis Transversal

**Pregunta T.1:** **Compare el enfoque de control de la Actividad 2 (velocidad hardcodeada) con el de la Actividad 3 (control por comandos UART).** ¿Qué ventaja concreta ofrece el segundo enfoque en el contexto de un experimento físico donde se necesita ajustar parámetros sin interrumpir la adquisición de datos? ¿Qué componente del sistema fue el que eliminó la restricción de recompilar? ¿Qué cambió arquitectónicamente entre los dos enfoques?

> [Respuesta del estudiante aquí]

**Pregunta T.2:** **Con base en la gráfica de la Actividad 4:** ¿La relación RPM vs duty cycle en la zona lineal es aproximadamente proporcional? Estime la pendiente (RPM por unidad de duty cycle) usando dos puntos de la zona lineal y determine si el ajuste es adecuado para un control proporcional simple.

> [Respuesta del estudiante aquí]

---

## 4. Código Documentado


### Actividad 2 — Motor DC: control de velocidad y dirección

```cpp
// Pegar aquí el código de la Actividad 2 (control de motor DC).
// Comentar: configuración de pines, lógica de dirección (IN1/IN2),
// control de velocidad con analogWrite(ENA, ...) y lectura del botón.
```

### Actividad 3 — Control UART: parser y protocolo

```cpp
// Pegar aquí el código de la Actividad 3 (parser UART).
// Comentar: lectura serial no bloqueante con Serial.available(),
// acumulación en buffer char buf[16], identificación de comandos
// con buf[0] y buf[1], extracción de parámetro con atoi(buf + 3),
// y respuesta de confirmación por Serial para cada comando.
```

### Actividad 4 — Encoder óptico: contador de pulsos y cálculo de RPM

```cpp
// Pegar aquí el código de la Actividad 4 (encoder + RPM).
// Comentar: ISR attachInterrupt() con variable volatile,
// cálculo de RPM con millis() cada 2 segundos,
// fórmula RPM = (contadorPulsos / N_franjas) × (60 / 2),
// e integración con el protocolo UART de Act. 3.
```

---

## 5. Dificultades Encontradas y Soluciones Aplicadas


### Dificultad 1: no sobre pasar los 500mA sugeridos para el motor DC

- **Síntoma observado:** _(¿Qué ocurrió exactamente?)_
- **Causa identificada:** _(¿Por qué ocurrió?)_
- **Solución aplicada:** _(¿Cómo lo resolvieron?)_
- **Lección aprendida:** _(¿Qué cambiarían la próxima vez?)_

### Dificultad 2 (si aplica): [Descripción breve del problema]

- **Síntoma observado:**
- **Causa identificada:**
- **Solución aplicada:**
- **Lección aprendida:**

---

## 6. Pregunta Abierta


**Pregunta:** La curva de caracterización de la Actividad 4 fue obtenida sin carga mecánica en el eje del motor. Proponga cómo cambiaría la curva si se aplica una carga de fricción constante al eje (por ejemplo, frenando el disco encoder con un dedo levemente). ¿En qué dirección se desplazaría la zona muerta? ¿Cómo podría usarse esa diferencia para estimar el torque de fricción del sistema, conociendo las especificaciones del motor?

> [Respuesta del estudiante aquí]