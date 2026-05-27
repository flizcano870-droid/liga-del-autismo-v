# Informe de Laboratorio — Sesión 9: Generación de Señales y Sistema Integrado

---

**Universidad Nacional de Colombia**
**Electrónica Digital — 2016684 — 2026-1**
**Prof. Ricardo Amézquita Orozco**

---

| Campo | |
|-------|--|
| **Integrantes** | 1. |
| | 2. |
| | 3. |
| | 4. |
| | 5. |
| **Grupo** | |
| **Fecha de la práctica** | Miércoles 22 de Abril, 2026 |
| **Fecha de entrega** | Viernes 25 de Abril, 2026 |

---

## 1. Resultados

### 1.1 Reto 1 — Generador de Señales con MCP4725

#### Captura 1: Forma de onda periódica

Coloque aquí una captura del osciloscopio mostrando una forma de onda periódica
(diente de sierra, triangular o senoidal) con amplitud ~0–5 V.

![Captura — Forma de onda periódica](imagenes/reto-1-onda-periodica.png)

#### Capturas 2–4: Tres formas de onda

Coloque aquí tres capturas del osciloscopio, una por cada forma de onda implementada.

![Diente de sierra](imagenes/reto-1-diente-sierra.png)

![Triangular](imagenes/reto-1-triangular.png)

![Senoidal con LUT](imagenes/reto-1-senoidal.png)

#### Captura 5: Control de frecuencia

Coloque aquí dos capturas del osciloscopio mostrando las dos condiciones extremas
del potenciómetro: frecuencia mínima (~0.5 Hz) y frecuencia máxima (~15 Hz).

![Frecuencia mínima](imagenes/reto-1-freq-min.png)

![Frecuencia máxima](imagenes/reto-1-freq-max.png)

---

### 1.2 Reto 2 — Caracterización I-V de LEDs con FSM

#### Tabla 1 — Datos I-V de muestra

Incluya las primeras 10 filas y las últimas 10 filas de uno de sus archivos CSV.
Las últimas 10 filas deben incluir la fila con el valor máximo de V_DAC (5.00 V).

**Primeras 10 filas:**

| V_DAC (V) | V_A1 (V) | V_LED (V) | I (mA) |
|:---------:|:--------:|:---------:|:------:|
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |

**Últimas 10 filas:**

| V_DAC (V) | V_A1 (V) | V_LED (V) | I (mA) |
|:---------:|:--------:|:---------:|:------:|
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |
| | | | |

#### Captura: Estados de la FSM en el OLED

Coloque aquí una foto del OLED mostrando cada uno de los tres estados de la FSM
(ESPERA, SUBIDA, COMPLETADO) durante la operación del Reto 2.

![OLED — Estados de la FSM](imagenes/reto-2-oled-fsm.png)

---

## 2. Visualización

### Gráfica 1 — Curva I-V del LED rojo

Graficar todos los datos del barrido completo del LED rojo (≥ 50 filas del CSV).

**Eje X:** V_LED (V)
**Eje Y:** I (mA)

![Curva I-V LED rojo](imagenes/grafica-1-iv-rojo.png)

**Interpretación:**

> [Identifique el voltaje umbral (V_th) donde la corriente comienza a crecer
> apreciablemente. Describa la forma de la curva y por qué es consistente con
> el comportamiento exponencial de una unión p-n.]

---

### Gráfica 2 — Comparación I-V: LED rojo vs LED verde

Superponer las curvas I-V del LED rojo y del LED verde en un mismo gráfico.

**Eje X:** V_LED (V)
**Eje Y:** I (mA)

![Comparación I-V rojo vs verde](imagenes/grafica-2-iv-comparacion.png)

**Interpretación:**

> [Identifique los voltajes umbral (V_th) para cada color (~1.8 V rojo, ~2.1 V
> verde). Explique por qué la diferencia de voltaje umbral es consistente con
> la relación entre el color (longitud de onda) y el bandgap del semiconductor.]

---

## 3. Análisis

**Pregunta 1 (Reto 1):** Deduzca la fórmula que relaciona la frecuencia de la señal
senoidal con el número de puntos N de la LUT y el tiempo entre puntos controlado
por el potenciómetro. Con N = 64, ¿cuál es el tiempo entre puntos necesario para
obtener 1 Hz? ¿Y para 15 Hz?

> [Respuesta aquí]

---

**Pregunta 2 (Reto 2):** ¿Por qué la corriente no crece linealmente con el voltaje
en el LED? Relacione la forma de la curva I-V con el modelo físico de una unión p-n.

> [Respuesta aquí]

---

**Pregunta 3:** Compare los dos métodos de generación de ondas periódicas que usó
en el Reto 1: barrido lineal (diente de sierra y triangular) versus Lookup Table
precalculada (senoidal). ¿En qué situaciones es preferible una LUT sobre un cálculo
en tiempo real, y viceversa? Fundamente con base en la precisión temporal, el uso
de memoria y la flexibilidad de cambiar parámetros.

> [Respuesta aquí]

---

**Pregunta 4:** En el Reto 2, la transición SUBIDA → FIN es automática (DAC == 4095),
mientras que ESPERA → SUBIDA y FIN → ESPERA dependen del botón. ¿Qué propiedad de
la FSM demuestra esta diferencia en los tipos de transición? ¿Cómo se modificaría
el diseño si todas las transiciones dependieran del botón — qué funcionalidad se perdería?

> [Respuesta aquí]

---

## 4. Código Documentado

Incluya SOLO el código que usted modificó o escribió. No incluya el código base
original ni el I2C Scanner. Comente cada bloque funcional.

### Reto 1 — Generador de Señales (lab-09-generacion-senales.ino)

```cpp
// Pegar aquí el código final del generador, con comentarios explicando:
//   - Cómo se implementó cada forma de onda
//   - Cómo funciona la FSM de selección
//   - Cómo se conecta el potenciómetro al control de frecuencia
```

### Reto 2 — Caracterización I-V con FSM (lab-09-iv-led.ino)

```cpp
// Pegar aquí el código completo del Reto 2, con comentarios explicando:
//   - La lógica de cada estado de la FSM
//   - Las condiciones de transición entre estados
//   - El cálculo de V_DAC, V_A1, V_LED e I
//   - El formato de salida CSV
```

---

## 5. Dificultades Encontradas y Soluciones Aplicadas

### Dificultad 1

- **Síntoma observado:**
- **Causa identificada:**
- **Solución aplicada:**
- **Lección aprendida:**

### Dificultad 2

- **Síntoma observado:**
- **Causa identificada:**
- **Solución aplicada:**
- **Lección aprendida:**

---

## 6. Pregunta Abierta

**Pregunta:** Proponga una extensión del sistema integrado (Reto 1 + Reto 2) que
utilice simultáneamente las capacidades de generación de señales y de caracterización
I-V. Por ejemplo: usar el generador para excitar un LED con una señal triangular y
medir la respuesta I-V resultante sin necesidad de un barrido por software paso a
paso. Describa qué modificaciones requerirían el hardware y el código, y qué ventaja
ofrecería este enfoque frente a la implementación actual.

> [Respuesta aquí]
