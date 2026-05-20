# Informe de Laboratorio — Sesión 8: Procesamiento de Señales para Mediciones

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
| **Fecha de la práctica** | |
| **Fecha de entrega** | Viernes 25 de Abril, 2026 — 23:59 (Informe Bloque 3) |

---

## 1. Resultados

### Actividad 1 — Filtrado Digital: Media Móvil y Filtro IIR

**Tabla 1A — Media móvil: efecto de N (f = 5 Hz)**

| N | Amplitud p-p cruda | Amplitud p-p (MM) | Atenuación (p-p_MM / p-p_cruda) |
|:--:|:------------------:|:-----------------:|:-------------------------------:|
| 4 | | | |
| 8 | | | |
| 16 | | | |

*f_s ≈ 50 Hz. El retardo teórico de la media móvil es (N−1)/2 muestras; para N = 16 (~7.5 muestras, 150 ms) debería apreciarse visualmente en el Serial Plotter.*

**Tabla 1B — Media móvil: efecto de la frecuencia (N = 8)**

| f (Hz) | Amplitud p-p cruda | Amplitud p-p (MM) | Atenuación | f / f_c |
|:------:|:------------------:|:-----------------:|:----------:|:-------:|
| 1.0 | | | | |
| 2.5 | | | | |
| 5.0 | | | | |
| 7.5 | | | | |
| 10.0 | | | | |

*N = 8 | f_s ≈ 50 Hz | f_c ≈ 2.77 Hz.*

**Tabla 1C — Comparación IIR vs. Media Móvil (α = 0.29, f_c ≈ 2.75 Hz)**

| f (Hz) | Amplitud p-p (IIR) | Atenuación IIR | Atenuación MM (de Tabla 1B) |
|:------:|:------------------:|:--------------:|:---------------------------:|
| 1.0 | | | |
| 2.5 | | | |
| 5.0 | | | |
| 7.5 | | | |
| 10.0 | | | |

*α = 0.29 | f_s ≈ 50 Hz | f_c ≈ 2.75 Hz (fórmula exacta).*

---

### Actividad 2 — Oversampling del LM35

**Tabla 2A — Efecto de N sobre la reducción de ruido**

| N | √N | 1/√N (teórico) | σ_over / σ_cruda (medido) |
|:--:|:--:|:--------------:|:-------------------------:|
| 1 | 1 | 1.00 | ≈ 1.00 |
| 4 | 2 | 0.50 | |
| 16 | 4 | 0.25 | |
| 64 | 8 | 0.125 | |
| 256 | 16 | 0.0625 | |

*σ_cruda se midió en el Paso 3 (N = 1, línea base). Para N = 1, σ_over = σ_cruda por definición. Para N > 1, ratio = σ_over / σ_cruda.*

---

## 2. Visualización

### Captura 1 — Serial Plotter: Filtrado digital en tiempo real

**Tipo:** Captura de pantalla del Serial Plotter del Arduino IDE

**Trazas:** `Cruda`, `MediaMovil`, `IIR` — simultáneas, etiquetadas
**Eje X:** tiempo (muestras)
**Eje Y:** valor ADC (unidades ADC, 0–1023)

**Condiciones requeridas en la captura:** señal senoidal a 5 Hz (200 mVpp, DC 2.5 V) y el momento del barrido de frecuencia (1 → 10 Hz).
**Lo que debe demostrarse:** a mayor frecuencia, mayor atenuación de ambos filtros; a igual f_c (~2.75 Hz), el IIR logra el mismo suavizado que la media móvil usando solo 1 variable.

![Captura 1 — Serial Plotter filtrado](imagenes/captura-1-serial-plotter-filtrado.png)

**Interpretación:** *(Describir qué muestra la captura. ¿Cómo cambia la amplitud de MediaMovil e IIR al aumentar la frecuencia? ¿Son similares las atenuaciones de ambos filtros a 5 Hz? ¿Se observa el retardo de la media móvil respecto a la señal cruda?)*

> [Respuesta del estudiante aquí]

---

### Gráfica 2 — Reducción de ruido por oversampling

**Tipo:** Gráfica de barras construida por el grupo (herramienta libre: Excel, Python, Google Sheets, etc.)

**Eje X:** N (1, 4, 16, 64, 256)
**Eje Y:** σ_over / σ_cruda
**Series:** Datos experimentales (barras) + curva teórica 1/√N (línea superpuesta)

**Lo que debe demostrarse:** el ratio medido decrece siguiendo la tendencia 1/√N. Para N = 16: ratio ≈ 0.25; para N = 256: ratio ≈ 0.0625.

![Gráfica 2 — Reducción de ruido oversampling](imagenes/grafica-2-oversampling.png)

**Interpretación:** *(A partir de la gráfica: ¿el ratio medido sigue la curva 1/√N? Si hay discrepancia, ¿en qué valores de N es mayor? ¿Qué podría explicarlo?)*

> [Respuesta del estudiante aquí]

---

## 3. Análisis

### Preguntas de Análisis

**Pregunta 1:**
*(Referencia: Tablas 1A y 1B)*

El filtro de media móvil con N = 16 suaviza más que con N = 4, pero también introduce mayor retardo. Durante la Actividad 1, al barrer la frecuencia del generador de 1 a 10 Hz, ¿cómo cambió la atenuación observada? A partir de f_c ≈ 0.443 × f_s / N, calcule la frecuencia de corte para N = 8 y N = 16 con f_s ≈ 50 Hz. ¿Concuerdan los valores calculados con lo observado en el Serial Plotter?

> [Respuesta del estudiante aquí]

---

**Pregunta 2:**
*(Referencia: Tabla 1C)*

El filtro IIR de primer orden usa la ecuación y = α·x + (1−α)·y_prev y solo necesita almacenar una variable. La media móvil requiere un buffer de N elementos. Compare ambos filtros a partir de la Tabla 1C: ¿son similares sus atenuaciones a cada frecuencia? ¿En qué situaciones del proyecto final preferiría uno sobre el otro? Considere la memoria disponible, la capacidad de cómputo y la respuesta en frecuencia de cada filtro.

> [Respuesta del estudiante aquí]

---

**Pregunta 3:**
*(Referencia: Tabla 2A y Gráfica 2)*

A partir de la Tabla 2A y la gráfica, ¿el ratio σ_over/σ_cruda sigue la curva teórica 1/√N? Si hay discrepancia, proponga al menos dos causas que la expliquen (considere la naturaleza del ruido del LM35 y las limitaciones del ADC). Para N grande (ej. 256), ¿el beneficio marginal justifica el costo en tiempo de muestreo?

> [Respuesta del estudiante aquí]

---

**Pregunta 4:**
*(Referencia: Actividades 1 y 2)*

Tanto la media móvil de la Actividad 1 como el oversampling de la Actividad 2 promedian N muestras para reducir ruido. ¿En qué se diferencian? Considere: ¿las muestras que promedia la media móvil son consecutivas o solapadas? ¿Qué implicaciones tiene esto sobre el retardo de la señal filtrada?

> [Respuesta del estudiante aquí]

---

## 4. Código Documentado

> **Nota:** En esta sesión no se suministró código base. Todo el código fue escrito desde cero por el grupo a partir de la teoría y los patrones de referencia de la guía. Incluir el código final de cada actividad con comentarios que expliquen la lógica implementada.

### Actividad 1 — Filtrado Digital (Generador de señales en A0)

```cpp
// Pegar aquí el código comentado de la Actividad de Filtrado Digital.
// Incluir: buffer circular, filtro de media móvil, filtro IIR,
// y los barridos de frecuencia con impresión etiquetada para Serial Plotter.
```

---

### Actividad 2 — Oversampling del LM35 (LM35 en A1)

```cpp
// Pegar aquí el código comentado de la Actividad de Oversampling.
// Incluir: acumulación de N lecturas con float, estadísticas periódicas
// (σ cruda y σ_over) con millis(), y barrido de N = 1, 4, 16, 64, 256.
```

---

## 5. Dificultades Encontradas y Soluciones Aplicadas

### Dificultad 1: [Descripción breve]

- **Síntoma observado:**
- **Causa identificada:**
- **Solución aplicada:**
- **Lección aprendida:**

---

### Dificultad 2: [Descripción breve] *(si aplica)*

- **Síntoma observado:**
- **Causa identificada:**
- **Solución aplicada:**
- **Lección aprendida:**

---

## 6. Pregunta Abierta

**Pregunta:** En el proyecto final del curso, su grupo medirá una o más variables físicas con sensores analógicos. Seleccione una de las técnicas estudiadas en esta sesión (filtrado u oversampling) e indique: (a) a qué variable de su experimento la aplicaría, (b) qué parámetros elegiría (N del filtro o N del oversampleo) y por qué, y (c) qué evidencia cuantitativa recopilaría para demostrar que la técnica mejoró la calidad de la medición.

> [Respuesta del estudiante aquí]
