# Informe de Laboratorio — Sesión 8: Procesamiento de Señales para Mediciones

---

**Universidad Nacional de Colombia**
**Electrónica Digital — 2016684 — 2026-1**
**Prof. Ricardo Amézquita Orozco**

---

| Campo | |
|-------|--|
| **Integrantes** | 1.Felipe Lizcano Quimbaya |
| | 2. Sergio Andres Poveda Perez|
| | 3.Sara Romero Chaves |
| | 4. Simon Sandoval Palma |
| **Grupo** |3|
| **Fecha de la práctica** |20 de mayo  |
| **Fecha de entrega** | Viernes 25 de Abril, 2026 — 23:59 (Informe Bloque 3) |

---

## 1. Resultados

### Actividad 1 — Filtrado Digital: Media Móvil y Filtro IIR

**Tabla 1A — Media móvil: efecto de N (f = 5 Hz)**

| N | Amplitud p-p cruda | Amplitud p-p (MM) | Atenuación (p-p_MM / p-p_cruda) |
|:--:|:------------------:|:-----------------:|:-------------------------------:|
| 4 |40| 19.75| 0.494|
| 8 | 39| 5.25|0.135 |
| 16 | 40| 5.19| 0.130|

*f_s ≈ 50 Hz. El retardo teórico de la media móvil es (N−1)/2 muestras; para N = 16 (~7.5 muestras, 150 ms) debería apreciarse visualmente en el Serial Plotter.*

**Tabla 1B — Media móvil: efecto de la frecuencia (N = 8)**

| f (Hz) | Amplitud p-p cruda | Amplitud p-p (MM) | Atenuación | f / f_c |
|:------:|:------------------:|:-----------------:|:----------:|:-------:|
| 1.0 |41 | 37| 0.902| 0.361|
| 2.5 | 41| 20.38| 0.500 | 0.920|
| 5.0 |40 | 6.5| 0.163| 1.805|
| 7.5 | 40| 3.88| 0.097| 2.708|
| 10.0 | 38| 5.63| 0.148| 3.610|

*N = 8 | f_s ≈ 50 Hz | f_c ≈ 2.77 Hz.*

**Tabla 1C — Comparación IIR vs. Media Móvil (α = 0.29, f_c ≈ 2.75 Hz)**

| f (Hz) | Amplitud p-p (IIR) | Atenuación IIR | Atenuación MM (de Tabla 1B) |
|:------:|:------------------:|:--------------:|:---------------------------:|
| 1.0 | 32.71|0.798 | 0.902|
| 2.5 | 18.93|0.473 | 0.500|
| 5.0 | 10.48|0.262 | 0.163|
| 7.5 | 7.72|0.203 | 0.097|
| 10.0 | 6.71|0.172 | 0.148|

*α = 0.29 | f_s ≈ 50 Hz | f_c ≈ 2.75 Hz (fórmula exacta).*

---

### Actividad 2 — Oversampling del LM35

**Tabla 2A — Efecto de N sobre la reducción de ruido**

| N | √N | 1/√N (teórico) | σ_over / σ_cruda (medido) |
|:--:|:--:|:--------------:|:-------------------------:|
| 1 | 1 | 1.00 | ≈ 1.00|
| 4 | 2 | 0.50 | 0.469|
| 16 | 4 | 0.25 |0.305 |
| 64 | 8 | 0.125 | 0.188|
| 256 | 16 | 0.0625 | 0.091|

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

![Captura 1 — Serial Plotter filtrado](5hz.jpeg)
5Hz
![Captura 2 — Serial Plotter filtrado](barrido.jpeg)
Barrido (1 → 10 Hz)

**Interpretación:** *(Describir qué muestra la captura. ¿Cómo cambia la amplitud de MediaMovil e IIR al aumentar la frecuencia? ¿Son similares las atenuaciones de ambos filtros a 5 Hz? ¿Se observa el retardo de la media móvil respecto a la señal cruda?)*

> la amplitud empezóa disminuir a medida que se aumenta la frecuencia, sinembargo no es necesariamente lineal.  El IIR se desfasó meno que el MM, sin embargo el MM presentó a lo largp del barrido una aplitud mayor qur el IIR.Por otro lado, a 5Hz las atenuaciones son similares, aunque, como se ve el la primera captura, el IIR tiene menos atenuación pues abarca una amplitud mayor. De la misma forma, el MM presenta mayor retardo con respecto a la señal cruda que el IIR, lo cual se evidencia en la primera captura.

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

> [Al realizar el barrido de frecuencia del generador desde 1.0 Hz hasta 10.0 Hz, se hace evidente que a medida que aumenta la frecuencia de la señal, el filtro produce una mayor atenuación. Según los datos de la Tabla 1B para un tamaño de buffer de N = 8, a 1.0 Hz la señal pasa casi intacta, presentando una atenuación de 0.902. Sin embargo, al subir a 2.5 Hz, la amplitud se reduce a la mitad (atenuación de 0.500). A partir de los 5.0 Hz, el bloqueo de la señal es mucho más severo (cayendo a 0.163), y llega a 0.148 cuando alcanza los 10.0 Hz. Para hallar la frecuencia de corte teórica, utilizamos la ecuación proporcionada en la guía: $f_c \approx \frac{0.443 \times f_s}{N}$. Sabiendo que la frecuencia de muestreo de las lecturas es de aproximadamente 50 Hz, podemos reemplazar los datos en la fórmula. Para un buffer de N = 8, el cálculo nos da una frecuencia de corte de 2.77 Hz , lo cual coincide exactamente con las notas de referencia de las tablas del laboratorio. Por otro lado, si aplicamos la misma fórmula para un buffer de N = 16, la frecuencia de corte resulta ser mucho más restrictiva, cayendo a aproximadamente 1.38 Hz.Estos valores teóricos calculados concuerdan perfectamente con el comportamiento registrado. Para N = 8, con una frecuencia de corte de 2.77 Hz , observamos en la tabla que justo antes de este umbral (a 2.5 Hz), la amplitud ya ha disminuido a la mitad. Al superar esta frecuencia (5.0 Hz y 7.5 Hz), la atenuación cae abruptamente a valores muy bajos, confirmando que el filtro bloquea esas frecuencias eficazmente. Por su parte, para N = 16, el filtro tiene una frecuencia de corte teórica de 1.38 Hz. Si revisamos la Tabla 1A a una frecuencia fija de 5.0 Hz, se ve una atenuación muy fuerte de 0.130. Como 5.0 Hz está muy por encima del corte de 1.38 Hz, es totalmente lógico que atenúe la señal con mucha más fuerza que un filtro de menor tamaño, confirmando que a mayor valor de N, el suavizado es mucho mayor.]

---

**Pregunta 2:**
*(Referencia: Tabla 1C)*

El filtro IIR de primer orden usa la ecuación y = α·x + (1−α)·y_prev y solo necesita almacenar una variable. La media móvil requiere un buffer de N elementos. Compare ambos filtros a partir de la Tabla 1C: ¿son similares sus atenuaciones a cada frecuencia? ¿En qué situaciones del proyecto final preferiría uno sobre el otro? Considere la memoria disponible, la capacidad de cómputo y la respuesta en frecuencia de cada filtro.

> [Cerca de la frecuencia de corte (~2.75 Hz), ambos filtros atenúan de forma casi idéntica. Sin embargo, a frecuencias altas (10 Hz), el IIR continúa atenuando de manera suave y constante, mientras que la media móvil pierde un poco de eficacia debido a un leve rebote natural en su comportamiento.El filtro IIR es el mejor en ahorro de memoria, ya que solo necesita almacenar un dato del pasado , mientras que la media móvil requiere guardar un bloque (buffer) de N datos. En contraparte, la media móvil es más rápida de procesar matemáticamente porque solo usa sumas sencillas , mientras que el IIR requiere multiplicar números con decimales, lo cual le exige un poco más de esfuerzo al procesador del Arduino.Elegiría el filtro IIR para  limpiar ruido de alta frecuencia de forma predecible y estable sin usar tanta memoria.]

---

**Pregunta 3:**
*(Referencia: Tabla 2A y Gráfica 2)*

A partir de la Tabla 2A y la gráfica, ¿el ratio σ_over/σ_cruda sigue la curva teórica 1/√N? Si hay discrepancia, proponga al menos dos causas que la expliquen (considere la naturaleza del ruido del LM35 y las limitaciones del ADC). Para N grande (ej. 256), ¿el beneficio marginal justifica el costo en tiempo de muestreo?

> [Al analizar los datos de la Tabla 2A, se puede observar que el ratio medido $\sigma_{over}/\sigma_{cruda}$ efectivamente disminuye a medida que aumenta N, siguiendo la tendencia general de la curva teórica $1/\sqrt{N}$, pero no se ajusta a ella a la perfección. A medida que el valor de N se hace más grande, el ratio medido resulta ser consistentemente mayor que el valor teórico esperado; por ejemplo, para N = 16 el valor medido es de 0.305 frente al teórico de 0.25, y para N = 256 el medido se estanca en 0.091 frente al 0.0625 que dicta la teoría. Esto demuestra que, aunque la técnica de oversampling reduce el ruido, su rendimiento real tiene un límite y se degrada respecto al ideal matemático.Esta discrepancia ocurre porque el modelo teórico asume que el ruido de la señal es perfectamente "blanco" y no correlacionado, es decir, totalmente aleatorio e independiente en cada muestra. En un sistema real, podemos explicar el fallo de este modelo a través de dos causas principales. Primero, por la naturaleza del propio ruido del LM35; los sensores reales suelen presentar ruido de baja frecuencia (como el ruido rosa o flicker noise) o ligeras derivas térmicas.  Como este tipo de ruido no fluctúa tan rápido ni es puramente aleatorio, tomar muestras consecutivas muy rápido captura "el mismo error" varias veces, haciendo que el promedio no lo elimine eficientemente. Segundo, por las limitaciones del ADC; el convertidor del microcontrolador tiene ruido de cuantización y no es perfectamente lineal. Si el ruido de la señal analógica es muy pequeño y no logra hacer "saltar" la lectura entre al menos un par de valores digitales adyacentes (LSB), el oversampling no puede realizar correctamente el promediado fraccional. Además, cualquier leve inestabilidad en el voltaje de referencia afectará a todo el bloque de muestras por igual, introduciendo ruido correlacionado.En cuanto a la viabilidad de utilizar un valor de N muy grande (por ejemplo, 256), el beneficio marginal obtenido rara vez justifica el costo en el tiempo de muestreo. Al revisar la Tabla 2A, saltar de N = 64 a N = 256 multiplica por cuatro el tiempo que el microcontrolador pasa leyendo el sensor, pero el ruido medido apenas se reduce de 0.188 a 0.091. Tomando en cuenta que 256 lecturas ininterrumpidas le toman al Arduino unos 26 ms, esa leve mejora extra en la limpieza de la medición no compensa la latencia introducida y el bloqueo del procesador, a menos que tu aplicación sea de naturaleza extremadamente estática y requieras la máxima resolución de voltaje posible sin importar qué tan lenta se vuelva la actualización de los datos.]

---

**Pregunta 4:**
*(Referencia: Actividades 1 y 2)*

Tanto la media móvil de la Actividad 1 como el oversampling de la Actividad 2 promedian N muestras para reducir ruido. ¿En qué se diferencian? Considere: ¿las muestras que promedia la media móvil son consecutivas o solapadas? ¿Qué implicaciones tiene esto sobre el retardo de la señal filtrada?

> [La diferencia fundamental entre la media móvil y el oversampling radica en la forma en que agrupan y procesan los datos. La media móvil utiliza una ventana deslizante, generalmente implementada mediante un buffer circular, lo que significa que promedia muestras solapadas. En cada ciclo de lectura, el sistema toma una muestra nueva, descarta la más vieja del historial y calcula un nuevo promedio de manera inmediata.  Por otro lado, el oversampling trabaja procesando bloques de muestras estrictamente consecutivas y no solapadas. Este método requiere tomar un grupo completo de $N$ lecturas nuevas seguidas, sumarlas o promediarlas para entregar un único valor de alta resolución, y luego reiniciar por completo el acumulador para comenzar a capturar el siguiente bloque desde cero.Esta diferencia estructural tiene implicaciones directas y muy distintas en el retardo de la señal filtrada. En la media móvil, al retener constantemente un historial de muestras antiguas mezcladas con la información más reciente, se genera un retardo continuo o desfase. Si se observa la señal, esta mantiene la misma frecuencia de salida o tasa de actualización que la original, pero su forma de onda se percibe desplazada o retrasada en el tiempo de forma constante.En contraste, el oversampling no genera este desfase continuo en la forma de onda, sino que introduce un tiempo de espera o latencia por cada bloque procesado. Debido a que el microcontrolador está obligado a recolectar las $N$ muestras completas antes de poder arrojar un solo resultado válido, la frecuencia a la que salen los datos disminuye drásticamente (un proceso conocido como decimación). Por lo tanto, el retardo en el oversampling corresponde exactamente al tiempo de espera que le toma al sistema capturar todo el bloque de lecturas antes de poder actualizar el valor final en la salida.]

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
