# Informe de Laboratorio — Sesión 2: Tiempo y Eventos — Mediciones Precisas con Arduino

---

**Universidad Nacional de Colombia**
**Electrónica Digital — 2016684 — 2026-1**
**Prof. Ricardo Amézquita Orozco**

---

| Campo | |
|-------|--|
| **Integrantes** |  Felipe Lizcano Quimbaya|
| |  Sergio Andrés Poveda Pérez |
| |  Sara Romero Chaves|
| |  Simon Gabriel Sandoval Palma|
| **Grupo** |#3|
| **Fecha de la práctica** | Miércoles 11 de Febrero, 2026 |
| **Fecha de entrega** | Miércoles 11 de Marzo, 2026 (Informe Bloque 1) |

---

## 1. Resultados

<!-- CRITERIO DE RÚBRICA: Resultados
     Nivel 2: Resultados completos y organizados
     Nivel 3: Con análisis estadístico o comparativo cuando aplique
     (Sesión de medición: tablas con N≥10, promedio, desv. estándar) -->

### Parte 1: Tiempo de Reacción (Polling)

| Medición | Tiempo millis() [ms] | Tiempo micros() [μs] | Diferencia [μs] |
|----------|----------------------|----------------------|------------------|
| 1 |217|215636|1364|
| 2 |227|227960|960|
| 3 |286|285284|716|
| 4 |199|198768|232|
| 5 |220|219660|340|
| 6 |180|179904|96|
| 7 |177|177352|352|
| 8 |269|268740|260|
| 9 |226|225208|792|
| 10 |166|166724|724|
| **Promedio** |216,7|216523,6|583,6|
| **Desv. Est.** |38,8|38502,6|395,9|

### Parte 2: Tiempo de Reacción (Interrupciones) + Bouncing

| Medición | Tiempo de reacción [μs] | contadorISR | Bouncing (sí/no) |
|----------|-------------------------|-------------|-------------------|
| 1 |202520|2|SI|
| 2 |202568|1|NO|
| 3 |247496|1|NO|
| 4 |166800|4|SI|
| 5 |299292|1|SI|
| 6 |156928|1|NO|
| 7 |297328|2|SI|
| 8 |241324|1|NO|
| 9 |233588|1|NO |
| 10 |218928|1|NO|
| **Promedio** |2266772| — | — |

### Parte 2: Experimento del Delay

| Valor del delay [ms] | Tiempo de reacción medido [μs] | Observaciones |
|-----------------------|-------------------------------|---------------|
| 10 (original) |2266772| |
| 100 |2247896| |
| 200 |2114972| |
| 500 |319246| |

### Parte 3: Período del Péndulo (Sensor IR)

**Longitud de cuerda L = ______ m**

| Medición | T medido [μs] | T medido [s] |
|----------|---------------|--------------|
| 1 | | |
| 2 | | |
| 3 | | |
| 4 | | |
| 5 | | |
| **Promedio** | | |
| **Desv. Est.** | | |

$T_{teórico} = 2\pi\sqrt{L/g}$ = ________ s

Error porcentual: _______ %

---

## 2. Visualización

<!-- CRITERIO DE RÚBRICA: Visualización
     Nivel 2: Figuras claras, rotuladas y referenciadas en el texto
     Nivel 3: Con interpretación cuantitativa o comparación con teoría -->

### Gráfica 1: Efecto del Delay sobre el Tiempo de Reacción (Interrupciones)

**Eje X:** Valor del delay en `loop()` (ms)
**Eje Y:** Tiempo de reacción medido (μs)

Diagrama de barras o puntos con los datos de la Tabla 3 (delay de 10, 100, 200 y 500 ms). Si el tiempo de reacción no cambia con el delay, eso demuestra que la ISR captura el evento independientemente de lo que haga el programa principal.

![Gráfica 1 — Tiempo de reacción vs delay](imagenes/grafica-1.png)

**Interpretación:** ¿El tiempo de reacción depende del valor del delay? ¿Qué demuestra esto sobre cómo funcionan las interrupciones?

> [Respuesta del estudiante aquí]

### Gráfica 2: T experimental vs T teórico (Péndulo)

Representar el valor medido de T (promedio ± desviación estándar) junto con el valor teórico $T = 2\pi\sqrt{L/g}$ para la longitud utilizada. Puede ser un diagrama de barras o un punto con barra de error y una línea horizontal para el valor teórico.

![Gráfica 2 — T experimental vs teórico](imagenes/grafica-2.png)

### Gráfica 3 (solo si se realizó el Reto 3): T² vs L

*Aplica únicamente si se midió el péndulo con ≥5 longitudes diferentes.*

**Eje X:** Longitud L (m)
**Eje Y:** Período al cuadrado T² (s²)

![Gráfica 3 — T² vs L](imagenes/grafica-3.png)

**Ecuación de ajuste lineal:** T² = _______ · L + _______

**Pendiente experimental:** _______ s²/m

**Pendiente teórica ($4\pi^2/g$):** _______ s²/m

**Valor de $g$ obtenido:** _______ m/s²

---

## 3. Análisis

<!-- CRITERIO DE RÚBRICA: Análisis (preguntas obligatorias)
     Nivel 2: Respuesta correcta respaldada por resultados propios
     Nivel 3: Con insight adicional o comparación cuantitativa -->

**Pregunta 1:** ¿Cuál es la diferencia promedio entre las mediciones de `millis()` y `micros()` en la Tabla 1? ¿Es consistente con la resolución de 1 ms de `millis()`?

> [Respuesta del estudiante aquí]

**Pregunta 2:** ¿Qué porcentaje de las pulsaciones en la Tabla 2 presentaron bouncing (`contadorISR > 1`)? ¿Cuál fue el valor máximo de `contadorISR` observado?

> [Respuesta del estudiante aquí]

**Pregunta 3:** Analiza los resultados de la Tabla 3: ¿el delay afectó el tiempo de reacción reportado? Explica por qué o por qué no, en términos del mecanismo de la ISR.

> [Respuesta del estudiante aquí]

---

## 4. Código Documentado

<!-- CRITERIO DE RÚBRICA: Código documentado
     Nivel 2: Código con comentarios básicos
     Nivel 3: Código limpio, bien comentado, explica la lógica -->

*Pegar el código completo de la Parte 3 con los 3 TODOs resueltos. Comentar cada bloque explicando la lógica implementada.*

### lab-02-parte3-pendulo.ino (TODOs completados)

```cpp
// Pegar aquí el código completo de la Parte 3 con los 3 TODOs resueltos y comentados
```

---

## 5. Dificultades Encontradas y Soluciones Aplicadas

<!-- CRITERIO DE RÚBRICA: Dificultades y soluciones
     Nivel 2: Describe problemas y cómo los resolvieron
     Nivel 3: Análisis de causa raíz + lección aprendida -->

### Dificultad 1: [Descripción breve]

- **Síntoma observado:**
- **Causa identificada:**
- **Solución aplicada:**
- **Lección aprendida:**

### Dificultad 2: [Descripción breve]

- **Síntoma observado:**
- **Causa identificada:**
- **Solución aplicada:**
- **Lección aprendida:**

---

## 6. Pregunta Abierta

<!-- CRITERIO DE RÚBRICA: Pregunta abierta
     Nivel 2: Propuesta viable y justificada
     Nivel 3: Propuesta creativa con análisis cuantitativo -->

**Pregunta:** Propón un experimento de física que podrías automatizar usando interrupciones y un sensor (IR, ultrasonido, u otro). Describe: qué mediría, qué sensor usaría, qué tipo de interrupción (RISING/FALLING/CHANGE), y qué ventaja tendría sobre una medición manual.

> [Respuesta del estudiante aquí]