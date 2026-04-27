# Informe de Laboratorio — Sesión 4: Comunicación Serial UART y Automatización con Python

---

**Universidad Nacional de Colombia**
**Electrónica Digital — 2016684 — 2026-1**
**Prof. Ricardo Amézquita Orozco**

---

| Campo | |
|-------|--|
| **Integrantes** | 1. Felipe Lizcano Quimbaya|
| | 2. Sergio Andres Poveda Perez |
| | 3. Sara Romero Chaves|
| | 4. Simon Gabriel Sandoval Palma|
| **Grupo** |3|
| **Fecha de la práctica** | Miercoles 22 de Abril 2026 |
| **Fecha de entrega** | Miércoles 8 de Abril, 2026 — 23:59 (Informe Bloque 2: S4, S5, S6) |

---

## 1. Resultados


### Actividad 2 — Medición de Tiempos de Transmisión

**Tabla 1 — Tiempos de Transmisión vs. Baudrate**

| Baudrate (baud) | Bytes enviados | $t_{\text{medido}}$ (µs) | $t_{\text{teórico}}$ (µs) | Error (%) | Justificación |
|:----------------|:--------------:|:------------------------:|:-------------------------:|:---------:|:--------------|
| 300 |57 |3399352 |1900000 |78.91 |Se desconoce la razon del porcentaje de error |
| 9600 | 57|105772 |59375 |78.14 | Se desconoce la razon del porcentaje de error|
| 57600 | 57|17024 |9896 |72.03 |Se desconoce la razon del porcentaje de error |
| 115200 | 57|8336 |4948 |68.47 |Se desconoce la razon del porcentaje de error |

*En "Bytes enviados" registra el número que reporta el sketch en su salida (campo `Bytes:`). Ese valor incluye los bytes del texto más los dos bytes del terminador `\r\n` agregados por `println()`. Usa ese mismo número para calcular $t_{\text{teórico}}$.*

*Fórmula teórica:*

$$t_{\text{teórico}} = \frac{N_{\text{bits}}}{f_{\text{baudrate}}} \quad \text{donde} \quad N_{\text{bits}} = \text{Bytes enviados} \times 10 \text{ bits/byte}$$

*Fórmula del error:*

$$\text{Error} = \frac{|t_{\text{medido}} - t_{\text{teórico}}|}{t_{\text{teórico}}} \times 100\%$$

---

### Actividad 3 — Verificación del Protocolo Humanizado

**Tabla 2 — Verificación del Protocolo Humanizado**

| Comando enviado | Terminador usado | Respuesta del Arduino | Acción observada en hardware | Observaciones |
|:----------------|:----------------:|:----------------------|:-----------------------------|:--------------|
| `STATUS` | Newline |led=off, botón=libre |led se mantiene apagado  | |
| `LED ON` | Newline |ok led encendido  |El led se encendio y se mantuvo encendido | |
| `LED OFF` | Newline |ok led apagado |se apago y se mantuvo asi  | |
| `BLINK 5` | Newline |ok parpadeando 5 veces...ok parpadeo terminado |El led se encendio y se apago 5 veces  | |
| `STATUS` (durante BLINK) | Newline |status led=off, , boton=libre |El led se encendio y se apago 5 veces | El resultado de status apareció al finalizar el proceso de blink 5|
| `FOO` | Newline | | | |
| `STATUS` | Sin terminador | nada|nada | No apareció nada en el serial monitor y no ocurrió nada con el circuito. Al colocar nuevamente el new line, aparecen los "status" colocados durante el no ending line, como si se guardaran para enviarse cuando se reestablece la conexión.|

---

### Actividad 4 — Comando COUNT

**Evidencia:** Pega a continuación la captura del Monitor Serial mostrando el comando `COUNT` funcionando (antes y después de pulsar el botón).

![COUNT funcionando](count.png)

> [Se reinicio el arduino para poder observar el conteo de pulsaciones desde 0, luego se hundió el botón 5 veces y se observa el conteo actualizado a 5 pulsaciones. ]

---

### Actividad 5 — Verificación del Protocolo Compacto

**Tabla 3 — Verificación del Protocolo Compacto**

| Trama enviada | Respuesta recibida | Acción en hardware | ¿Respuesta correcta? (Sí/No) |
|:---------------------|:-------------------|:-------------------|:-----------------------------:|
| `ST 00000` | L=0, B=0, E=0|sin cambios( led apagado) |si|
| `ON 00000` |OK 00000 |led encendido |si |
| `OF 00000` |OK 00000 |se apagó el led |si |
| `BL 00003` |OK 00000 |se encendió 3 veces el led |sí |
| `EV 00001` |OK 00005|se hundió el botón 5 veces |si |
| Trama mal formada |ERM 00000 |no afecta el circuito | sí|

---

### Actividad 6 — BLINK No Bloqueante

**Tabla 4 — Diagnóstico del BLINK Bloqueante vs. No Bloqueante**

| Prueba | Versión bloqueante | Versión no bloqueante |
|:-------|:-------------------|:----------------------|
| Pulsaciones registradas tras `BL 00005` (5 pulsaciones durante parpadeo) | 0| 5|
| Respuesta a `ST 00000` durante parpadeo: ¿llega inmediatamente? | no, espera a que termine el parpadeo para mandar el status | sí, es inmediata y registra los eventos ocurridos|
| ¿El LED completa el número solicitado de parpadeos? | sí | sí |

**Justificación evaluable:** ¿Resolvería `attachInterrupt()` **ambos** problemas observados (pulsaciones perdidas Y falta de respuesta a `ST` durante el parpadeo)?

> [No, si se corrigen las pulsaciones perdidas porque la interrupcion detecta el boton en el momento exacto, aunque el programa este "ocupado" con el delay pero no arregla la falta dfe respuesta a 'ST' mientras el arduino esta en el parapadeo con delay ya que el programa queda congelado y no pude leer ni responder comandos seriales y la interrupcion no sirve para leer estos comandos ]

---

### Actividad 7 — Terminal Crudo Python

**Tabla 5 — Intercambios desde Terminal Crudo Python**

| # | Trama enviada por Python | Respuesta recibida | ¿Correcto? |
|:--|:-------------------------|:-------------------|:----------:|
| 1 | ST 00000 | OK L=0, B=0, E=0 | sí |
| 2 | BL 00006 | OK 00000 | sí |
| 3 | ON 00000 | OK 00000 | sí |
| 4 | EV 00001 | OK 00000 | sí |
| 5 | OF 000 | ER 00000 | sí |

*Incluye al menos una trama mal formada entre los intercambios registrados.*

---

## 2. Visualización


### Figura 1 — Monitor Serial: salida del sketch de medición (una captura por baudrate)

![Figura 1 — Salida Monitor Serial por baudrate](1.png)

**Caption:** Cuatro capturas del Monitor Serial, una por baudrate (300, 9600, 57600, 115200). Indicar en cada caption: baudrate, número de bytes transmitidos y texto enviado.

**Interpretación:** 

> [Podemos observar de la imagen anterior que efectivamente el  tmedido   disminuye mientras el baudrate aumenta lo que verifica que al aumentar el baudrate el tiempo de transmisión disminuye ]

---

### Figura 2 — Gráfica: $t_{\text{medido}}$ vs. $t_{\text{teórico}}$ por Baudrate

**Eje X:** Baudrate (baud) — escala logarítmica recomendada: 300, 9600, 57600, 115200
**Eje Y:** Tiempo de transmisión (µs)
**Series:** $t_{\text{medido}}$ (puntos sólidos) y $t_{\text{teórico}}$ (puntos vacíos o línea de referencia)

![Figura 2 — Gráfica t_medido vs t_teórico por baudrate](Figure_1.png)

**Lo que debe demostrar esta gráfica:** Que el tiempo de transmisión es inversamente proporcional al baudrate, y que el tiempo medido se ajusta al modelo teórico con un overhead sistemático pequeño y aproximadamente constante en valor absoluto.

**Interpretación:**

> [Podemos observar las rectas con pendiente negativa lo que precisamente nos confirma la relacion de proporcionalidad inversa entre el baudrate y el tiempo de transferencia, de igua manera se observa que la curva medida está siempre por encima de la teórica y la separación entre ambas es casi constante en el eje vertical (en µs), no proporcional y por lo tanto el overhead es aproximadamente constante en valor absoluto no en porcentaje.]

---

### Figura 3 — Monitor Serial: sesión del protocolo humanizado

![Figura 3 — Monitor Serial protocolo humanizado](3.png)

**Caption:** Captura del Monitor Serial mostrando al menos cuatro comandos del protocolo humanizado con sus respuestas. Identificar en el caption que el protocolo es humanizado.

**Interpretación:**

> [En la imagen podemos apreciar la respuesta de los comandos validos(respueta previamente programada en hardaware y software) e invalidos( Error: comando invalido). Durante el blink podemos ver en la consola el mensaje de que se esta realizando el proceso y cuando este ha finalizado, en cuanto al hardware efectivame se puede apreciar el parpadeo del LED ]

---

### Figura 4 — Consola Python: sesión del terminal crudo

![Figura 4 — Consola Python terminal crudo](captura_experimento_6.png)

**Caption:** Captura de la consola Python mostrando la sesión completa del terminal crudo con cinco o más intercambios. Indicar en el caption: puerto serial y baudrate usados.

**Interpretación:**

> [Se pudo apreciar que Python efectivamente envia y recibe las tramas de forma fijo ( CC NNNNN) y las respuestas del Arduino fueron las esperadas con cada comando valido (ON 00000, ST 00000, OFF 00000, BL 0000N, EV 00001) ]

---

## 3. Análisis


**Pregunta 1** *(Actividad 2 — Tiempos de transmisión):*

Calcula el error porcentual promedio entre $t_{\text{medido}}$ y $t_{\text{teórico}}$ para los cuatro baudrates. ¿El error es aproximadamente constante en valor absoluto (µs) o en valor relativo (%)? ¿Qué overhead sistemático del sistema podría explicar el patrón observado?

> [El error es aproximandamente constante en el valor relativo , con un promedio de error de 74.39% .Por parte de los experimentadores no se encontro una razon para justificar este promedio de error  ]

---

**Pregunta 2** *(Actividad 3 — Protocolo humanizado):*

Con base en la fila STATUS durante BLINK de la Tabla 2: ¿en qué momento exacto respondió el Arduino y por qué? Relaciona la respuesta con la estructura del `loop()` y el uso de `delay()`.

> [Cuando se hacen BLINK 5 se tiene un delay de 500ms, por lo tanto el arduino no responde a los comandos enviados durante ese tiempo, y responde al finalizar el proceso de BLINK 5, esto se debe a que el loop() se encuentra bloqueado por el delay() y no puede procesar los comandos enviados durante ese tiempo. ]

---

**Pregunta 3** *(Actividad 5 — Protocolo compacto):*

¿Por qué `cmd2()` es suficiente para identificar comandos en el protocolo compacto pero no lo sería en el protocolo humanizado?

> [Dado que cmd2() tiene una trama de formato fijo, con 2 caracteres para el comando y 5 para el argumento, no es necesario usar strcmp() para comparar toda la cadena, sino que con comparar los primeros 2 caracteres es suficiente para identificar el comando. En cambio, en el protocolo humanizado, los comandos pueden tener diferentes longitudes y formatos, por lo que es necesario usar strcmp() para comparar toda la cadena y asegurarse de que se está identificando correctamente el comando. ]

---

**Pregunta 4** *(Actividad 6 — BLINK no bloqueante):*

¿Qué consecuencias concretas tendría usar `delay(1)` (un milisegundo) en lugar de `delay(500)` en el BLINK bloqueante sobre la capacidad del sistema para responder comandos seriales? Estima la frecuencia máxima de comandos que el cliente podría enviar sin pérdida de respuestas en cada caso, asumiendo que cada comando tiene una longitud de 9 caracteres a 9600 baudios.

> [Usar un delay(1) en lugar de delay (500) reduciria drasticamente el tiempo durante el cual el sistema permanece bloqueado sin atender el puerto serial. A 9600 baudios, un comando de 9 caracteres tarda aproximadamente 9.4 ms en trasnmitirse. Con el delay(500), el Arduino puede permanecer hasta 500ms sin leer el buffer serial, en ese intervalo pueden llegar aproximadamete 50 comandos, lo que excede la capacidad del buffer y provoca perdida de datos. Por ello la frecuencia maxxima de comandos que se puede manejar sin perdida de datos es de aproximadamente 2 comandos por segundo. En cambio con delay(1), el sistema solo queda bloqueado por 1ms antes de volver a ejecutar el loop() y revisar el puertoo serial. Dado que este tiempo es mucho menor que e tiempo de llegada de un comando el sistema puede atender el buffer multipkes veces durante la recepcion de cada mensaje. En este caso la frecuencia maxima queda limitada por la velocidad de transmision, siendo aproximadamente 100 comandos por segundo, sin perdida significativa. ]

---

**Pregunta 5** *(Análisis transversal — PA1):*

¿Por qué un protocolo textual necesita un delimitador de línea explícito (`\n`) y no puede basarse en pausas de tiempo entre comandos?

> [Respuesta del estudiante aquí]

---

**Pregunta 6** *(Análisis transversal — PA2):*

En el protocolo humanizado, el parser usa `strcmp()` para identificar comandos. En el protocolo compacto, solo compara dos caracteres con `cmd2()`. ¿Cuál de los dos parsers sería más eficiente si el protocolo tuviera 50 comandos distintos? Justifica considerando el número de comparaciones necesarias en el peor caso.

> [Respuesta del estudiante aquí]

---

**Pregunta 7** *(Análisis transversal — PA3):*

Compara los parsers del protocolo humanizado y el protocolo compacto desde la perspectiva de un sistema embebido con recursos limitados (memoria y velocidad de CPU). ¿Cuál de los dos es más adecuado para una aplicación de producción y por qué?

> [Respuesta del estudiante aquí]

---

## 4. Código Documentado


### `lab-04-parte1-baudrate.ino` — Corrección de baudrate (Actividad 1)

```cpp
// Pega aquí la línea corregida de Serial.begin() con comentario explicativo.
```

### `lab-04-parte2-humanizado.ino` — Comando COUNT (Actividad 4)

```cpp
// Pega aquí las secciones que agregaste:
// 1. Declaración de variables
// 2. Lógica de detección de pulsaciones en loop()
// 3. La rama nueva en el parser
// 4. La respuesta formateada con Serial.println()
```

### `lab-04-parte3-compacto.ino` — BLINK no bloqueante (Actividad 6)

```cpp
// Pega aquí la implementación completa de updateBlink()
// y las modificaciones a la rama BL en procesarComando().
// Comenta cada variable de estado: blinkActivo, parpadeosPendientes,
// ultimoCambioMs, estadoLEDParpadeo.
```

### `cliente_menu.py` — Cliente con menú humanizado (Actividad 8, si aplica)

```python
# Pega aquí las opciones del menú que implementaste.
# Muestra al menos: cómo compones la trama compacta, cómo interpretas la respuesta.
```

---

## 5. Dificultades Encontradas y Soluciones Aplicadas


### Dificultad 1: [Describe brevemente el problema]

- **Síntoma observado:** [¿Qué veías? ¿Qué fallaba exactamente?]
- **Causa identificada:** [¿Por qué ocurrió?]
- **Solución aplicada:** [¿Qué hiciste para resolverlo?]
- **Lección aprendida:** [¿Qué harías diferente la próxima vez?]

### Dificultad 2 (si aplica): [Describe brevemente el problema]

- **Síntoma observado:**
- **Causa identificada:**
- **Solución aplicada:**
- **Lección aprendida:**

---

## 6. Pregunta Abierta


**Pregunta:** Propón una extensión del protocolo compacto para una sesión futura en la que el Arduino deba controlar dos actuadores (por ejemplo, un motor y un LED) y reportar dos sensores (temperatura y distancia). Especifica:

(a) Los nuevos comandos que agregarías con su formato de trama.

(b) Los nuevos tipos de respuesta necesarios.

(c) Cómo distinguirías en el protocolo las respuestas síncronas (a comandos) de los eventos asíncronos (cambios en sensores).

> [Respuesta del estudiante aquí — propuesta de extensión con justificación técnica]