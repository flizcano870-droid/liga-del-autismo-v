# Propuesta de Proyecto Final — Electrónica Digital 2026-1

**Grupo #:** _3_  
**Integrantes:**  
1. _Felipe Lizcano Quimbaya__  
2. _Sergio Andres Poveda Perez__  
3. _Sara Romero Chaves__  
4. _Simon Gabriel Sandoval Palma__  

**Fecha de entrega:** ___

---

> ⚠️ **Regla de oro:** Cada integrante debe poder explicar y defender cada decisión de diseño y cada línea de código. En cada entregable y en la presentación final se hará una pregunta aleatoria a cualquier miembro del grupo.

---

## 1. Descripción del Proyecto

**¿Qué van a construir?**

Se diseñará un sistema electrónico capaz de monitorear continuamente ciertas condiciones ambiantales (temperatura, humedad de la tierra, CO2, etc.) de un prototipo de invernadero.
El sistema tendrá procesos automatizados con sensores y actuadores, además de alertas de alertas visuales que indican la necesidad de realizar alguna acción de mantenimiento. Por lo tanto, el sistema influye y optimiza el cuidado de una planta. 

El prototipo busca aportar a la agricultura urbana, permitiendo facilitar el cuidado de cultivos alimenticios en espacios reducidos, como apartamentos o balcones. 

Los sensores utilizados serán:
- Sensor de temperatura 
- Sensor de humedad del suelo
- Sensor de luz (LDR)
- Sensor de calidad del aire 
- Sensor de conductividad eléctrica 
- Sensor ultrasónico

La salidas del circuito serán:
- Ventilador para controlar la temperatura
- Termoresistencia 
- Bomba de agua para sistema de riego automático
- LEDs indicadores de alerta (para índices de CO2 y nivel del agua en el tanque)
- Display OLED para mostrar datos en tiempo real
- Comunicación serial con PC
  
**Describan una sesión típica de uso:** 

El diseño se piensa como un sistema que debe funcionar prácticamente de forma autónoma, con alertas visuales para el usuario. El usuario solo debe interactuar para realizar tareas de mantenimiento (regar, abrir ventana, etc.) cuando el sistema lo indique. Luego de la instalación inicial, el dispositivo se encarga de monitorear y controlar las condiciones por sí mismo, el usuario solo debería revisar el estado de los LEDs y la información de la pantalla OLED para saber si es necesario realizar alguna acción. Por ejemplo, si el LED que indica que el sistema de riego necesita agua en su tanque está encendido, el usuario deberá llenarlo; por otro lardo, si se indican en la pantalla OLED bajos niveles de luz dentro del invernadero, el usuario deberá remover la parte superior de la estructura que se lo permite.


## 2. Solución Propuesta — Diagrama de Bloques

**¿Cómo resuelven el problema con el toolkit del curso?**

Dibujen un diagrama de bloques del sistema completo. Cada bloque debe ser un subsistema identificable con su función específica.

La arquitectura depende del tipo de proyecto. Algunos ejemplos:

**Tipo A — Instrumento de medición / automatización:**

![Diagrama de bloques circuito](Diagrama%20de%20bloques%20circuito.png)

**Tipo B — Dispositivo interactivo (instrumento musical, juego, controlador):**
```
┌──────────┐    ┌──────────┐    ┌──────────┐
│ ENTRADAS │ →  │ ARDUINO  │ →  │  SALIDAS │
│(botones, │    │(FSM,     │    │(sonido,  │
│ potenció-│    │ lógica)  │    │ LEDs,    │
│ metros)  │    │          │    │ display) │
└──────────┘    └──────────┘    └──────────┘
```

*Elijan la arquitectura que corresponda a su proyecto o dibujen una propia. Identifiquen cada bloque con su función específica. Si su proyecto usa comunicación serial con Python (S4), agreguen el bloque PC/LOG.*

---

## 3. Matriz de Trazabilidad — Toolkit del Curso

**¿Qué habilidades de S1-S9 usa cada subsistema de su proyecto?**

Marquen con ✅ y describan cómo aplican cada habilidad. Si un subsistema no usa cierta habilidad, déjenlo en blanco.

| Habilidad del curso | Subsistema 1: Monitoreo ambiental | Subsistema 2: Control ambiental | Subsistema 3: interfaz y registro |
|---|---|---|---|
| **S1** — E/S digital, protoboard |✅ Conexión de sensores|✅ Control digital y actuadores |✅ Leds de alerta|
| **S2** — Timing preciso, interrupciones |✅ Muestreo de sensores (millis) |✅ Activación de actuadores (no bloqueante) |✅ Actualización de display OLED (sin delay)|
| **S3** — Debouncing, ADC, sensores analógicos |✅ Lectura de sensores analógicos (ADC) | | |
| **S4** — Comunicación UART + Python | | |✅ Envío de datos ambientales a CCV |
| **S5** — PWM, H-Bridge, actuadores | |✅ Control PWM mediante MOSFET (ventilador bomba, resistencia) | |
| **S6** — Adquisición multicanal, OLED I2C |✅ Adquisición simultánea de sensores| |✅ Visualización de datos en OLED I2C|
| **S7** — Control PID | |✅ Control de la temperatura (termoresistencias) | |
| **S8** — Filtrado digital, oversampling, triggers |✅ Filtrado del ruido para sensores (humedad, conductividad y calidad del aire) | | |
| **S9** — DAC MCP4725, FSM, generación de señales | |✅ FSM para decidir qué actuador utilizar | |

> Si el proyecto completo usa menos de 4 habilidades distintas del curso, probablemente es demasiado simple. Pero no inflen la matriz: solo marquen las habilidades que REALMENTE usan.

---

## 4. Diseño Técnico

### 4.1 Diagrama Esquemático

![Diagrama del circuito](Fantastic%20Allis-Vihelmo.png)


Dibujen el esquemático completo mostrando:
- Todos los pines de Arduino utilizados
- Conexiones de sensores, actuadores, display, DAC
- Resistencias, capacitores y componentes de protección
- Alimentación (5V, 12V, GND)

*Pueden dibujarlo a mano (foto) o usar Fritzing/Tinkercad.*

### 4.2 Tabla de Pines

| Pin Arduino | Conectado a | Función |
|---|---|---|
| A0 | LM35 | Medición de temperatura |
| A1 | YL100 | Medición de la humedad del suelo |
| A2 | MQ- 135 | Medición de la calidad del aire |
| A3 | LDR | Medición de la intensidad luminosa |
| A4 | OLED SDA| Comuicación I2C | 
| A5 | OLED SCL| Comunicación I2C |
| D2 | Echo HC-SR04| Medición del nivel de agua del tanque|
| D3 | Trig HC-SR04 | Generación pulso ultrasónico|
| D4 | Led agua | Alerta de nivel bajo del agua |
| D5 | Led calidad del aire | Alerta de mala calidad del aire|
| D6 | Led luz | Alerta de baja luminosidad|
| D9 PWM| MOSFET ventilador | Control de ventilador (sist. enfriamiento) |
| D10 PWM| MOSFET bomba | Control de bomba de agua (sist. riego) |
| D11 PWM | MOSFET termoresistencia | Control de termoresistencia (sist. calefacción) |
|VIn | Bomba, resistencia calefactora, ventilador, diodos| Alimentación de energía |
|5.5V| Sensores, leds, oled | Alimentacion de emergia|
| GND | conectado a todos los módulos | Tierra común| 

### 4.3 Arquitectura de Software

Describan la máquina de estados (FSM) de su sistema. La FSM es la implementación concreta de S9 que ya identificaron en la matriz de trazabilidad (Sección 3). Aquí la detallan con diagrama y pseudocódigo:

![Diagrama de bloques de FSM](Diagrama%20de%20bloques%20de%20FSM.png)

Incluyan pseudocódigo de la FSM principal y las funciones clave:

```cpp
// Pseudocódigo — FSM principal
//=========================================
// FSM PRINCIPAL DEL INVERNADERO INTELIGENTE
//=========================================

enum Estado {
    INICIO,
    MONITOREO,
    CONTROL,
    ALARMA
};

Estado estado = INICIO;

void loop()
{
    switch(estado)
    {

        //=================================
        // ESTADO INICIO
        //=================================
        case INICIO:

            // Inicializar sensores
            inicializarLM35();
            inicializarYL100();
            inicializarMQ135();
            inicializarLDR();
            inicializarHCSR04();

            // Inicializar OLED
            inicializarOLED();

            // Inicializar comunicación serial
            inicializarUART();

            // Inicializar actuadores
            inicializarBomba();
            inicializarVentilador();
            inicializarTermoresistencia();

            estado = MONITOREO;

        break;


        //=================================
        // ESTADO MONITOREO
        //=================================
        case MONITOREO:

            // Lectura de variables ambientales
            leerTemperatura();
            leerHumedadSuelo();
            leerCalidadAire();
            leerLuminosidad();
            leerNivelAgua();

            // Filtrado digital de señales
            filtrarDatos();

            // Mostrar información en OLED
            mostrarOLED();

            // Enviar datos al PC mediante UART
            enviarDatosSerial();

            // Alarma por falta de agua
            if(nivelAgua < NIVEL_MINIMO)
            {
                estado = ALARMA;
            }

            // Verificar necesidad de control
            else if(
                humedadSuelo < HUM_MIN ||
                temperatura < TEMP_MIN ||
                temperatura > TEMP_MAX ||
                calidadAire > AIRE_MAX
            )
            {
                estado = CONTROL;
            }

        break;


        //=================================
        // ESTADO CONTROL
        //=================================
        case CONTROL:

            // Control de humedad del suelo
            controlarBomba();

            // Control de temperatura
            controlarVentilador();

            controlarTermoresistencia();

            // Indicadores luminosos
            actualizarLEDs();

            // Verificar tanque vacío
            if(nivelAgua < NIVEL_MINIMO)
            {
                estado = ALARMA;
            }

            // Regresar cuando todas las
            // variables estén en rango
            else if(
                humedadSuelo >= HUM_MIN &&
                temperatura >= TEMP_MIN &&
                temperatura <= TEMP_MAX &&
                calidadAire <= AIRE_MAX
            )
            {
                estado = MONITOREO;
            }

        break;


        //=================================
        // ESTADO ALARMA
        //=================================
        case ALARMA:

            // Apagar sistema de riego
            apagarBomba();

            // Encender LED de tanque vacío
            digitalWrite(LED_AGUA, HIGH);

            // Mostrar mensaje de alerta
            mostrarMensajeTanqueVacio();

            // Enviar alerta al PC
            Serial.println("ALARMA: Tanque de agua vacío");

            // Esperar recarga del tanque
            if(nivelAgua >= NIVEL_MINIMO)
            {
                digitalWrite(LED_AGUA, LOW);

                estado = MONITOREO;
            }

        break;
    }
}
```

> ⚠️ Este pseudocódigo es solo la estructura. En el código real deben usar `millis()` o interrupciones para el timing (S2). **Nada de `delay()` en una FSM.**

---

## 5. Lista de Materiales

| Componente | Cant. | Costo unit. | Costo total | Proveedor | Disponible? | Plan B si no se consigue |
|---|---|---|---|---|---|---|
| Arduino Uno | 1 | Ya tienen | $0 | Kit | ✅ | — |
| Sensor LM35 | 1 | Ya se tiene | $0 | Caja de componentes propia| ✅ | Termistor NTC + calibración manual |
| Sensor YL100 | 1 | $6000 | $6000 | Mactronica | ✅ | DHT22 + calibración |
| Sensor LDR | 1 | Ya se tiene | $0 | Caja de componentes propia | ✅  | GY-302|
| Pantalla OLED | 1 | Ya se tiene | $0 | Caja de componentes propia | ✅ | LCD 16x2 + multiplexor I2C |
| Sensor HC-SR04 | 1 | $5950 | $5950 | Sigma electrónica | ✅ | Sensor de nivel ultrasónico JSN-SR04T |
| LEDs (diferentes colores)| 3 |  $357 | $1071 | Sigma electrónica | ✅ | — |
| MOSFET IRLZ44N | 3 | $4284 | $12852 | Sigma electrónica | ✅ | NMOS |
| Sensor MQ-135 | 1 | $8925 | $8925 | Sigma electrónica | ✅ | Sensor de CO2 MH-Z19B |
|Resistencias | 6 (220 $\Omega$)  4 (10 K$\Omega$)| $200| $2000 | Chaza de electronica |✅|-
|Protoboard| 2 | Ya se tiene | $0 | $0 | Caja de componenetes propia|✅ |-
|Diodos| 2 | $150| $300 | Sigma electronica | ✅ |-
|Ventilador 12v|1| $7900| $7900 | Zamux | ✅ | motor con aspas|
|Bomba de agua |1|$10000|$10000|Zamux|✅|Bomba sumergible de acuario|
|Resistencia calefactora |1|$23000|$23000|yorobotics|✅|Resistencia de cartucho|
|Plantula de lechuga|3|$2000|$6000|Viveros de colombia|✅| Albahaca|
|Maceta|1| 48900$|$48900|Homecenter|✅|-|
|Tierra |1kg|$5500|$5500|Homecenter|✅|-
|Tubos de PVC y accesorios de PVC | tubo de PVC (6m) Tee de PVC (4 und) Codo de PVC (8und) |tubo $12900 Tee 640$ Codo $2100| $32260|Homecenter| ✅|-
|Balso| 2m | Ya se tiene | 0$| Caja de componentes propia|✅|-
|Tanque de agua (8lts) |1| $14900|$14900|Homecenter|✅|-
|Manguera 1/4" para bomba sumergible|1|$2900|$2900|zamux|✅|-


**Costo total estimado:** $187158

> ⚠️ **Crítico:** Si un componente es indispensable y no se consigue, el proyecto está en riesgo. Definan un Plan B para cada componente crítico.

---

## 6. Cronograma — Entregables

*7 semanas de ejecución. La Semana 1 empieza el mismo día en que se aprueba la propuesta. Tres entregables obligatorios. Cada uno debe ser una demo funcionando, no un documento.*

| Entregable | Semana | ¿Qué se demuestra? | Pre-requisitos | Definición de "terminado" |
|:---:|:---:|---|---|---|
| **Entregable 1** | Semana 2 | Estructura base y circuito funcional (midiendo) | Circuito previamente armado y código términado, además de tener los materiales de la maqueta ya armados | Tener los dos módulos principales del proyecto perfectamente terminados para empezar el acople de estos |
| **Entregable 2** | Semana 5 | Unión entre el circuito y la maqueta mostrando funcionalidad| Tener el entregable 1 completado, además de las maquetas de los dos módulos construidas totalmente | Cuando ya estén en acción medición de los sensores y actuadores sobre el pototipo de invernadero |
| **Entregable 3 (Final)** | Semana  7 | Sistema completo funcionando + defensa | entregable 1 y 2 + un avance del crecimiento del cultivo  | El sistema demuestra funcionalmente el monitoreo de la planta, a su vez actúa y avisa cuando las condiciones internas lo requieren |

**Para cada entregable, definan:**
- ¿Qué subsistemas estarán funcionando?
- ¿Qué evidencia van a mostrar? (demo en vivo, datos, gráficas)
- **Pre-requisitos:** ¿Qué debe estar listo ANTES de empezar este entregable?
- **Definición de "terminado":** ¿Cómo se sabe, sin ambigüedad, que este entregable está completo?
- ¿Qué riesgos podrían retrasar este entregable y cómo los mitigan?

> Si un entregable se retrasa, ¿afecta a los demás? Si el Entregable 1 depende de un componente que no llegó, ¿hay Plan B?

> 🔄 **Cambio de rumbo:** Si después del Entregable 1 descubren que el proyecto no es viable, pueden proponer un ajuste del alcance o un pivoteo justificado — pero DEBEN discutirlo con el profesor ANTES del Entregable 2. No se aceptan cambios de proyecto en la última semana.

---

## 7. Producto Mínimo Viable (MVP)

> 💀 **Si TODO sale mal** — no llega un componente, se enferma un integrante, se daña algo...

**¿Cuál es la demostración MÁS PEQUEÑA que prueba que aprendieron?**

**Nuestro MVP es:**

Un sistema capaz de monitorear temperatura, humedad del suelo, intensidad lumínica y nivel de agua del tanque, mostrando la información en una pantalla OLED y enviándola por comunicación serial al computador. Además, el sistema generará alertas visuales mediante LEDs cuando se detecten condiciones fuera de los rangos establecidos para el cultivo.

---

**Prueba de aceptación del MVP:** ¿Qué evidencia concreta demostraría que el MVP está completo?

El MVP se considerará completado cuando:

1. El sensor LM35 mida la temperatura y la muestre correctamente en la pantalla OLED con una actualización máxima de 1 segundo.
2. El sensor YL-100 reporte el nivel de humedad del suelo y este valor sea visible tanto en la OLED como en el monitor serial.
3. El sensor LDR detecte cambios de iluminación, activando un LED de alerta cuando la intensidad lumínica se encuentre por debajo del umbral establecido.
4. El sensor ultrasónico HC-SR04 mida el nivel de agua del tanque y active un LED de alerta cuando el nivel sea inferior al mínimo definido.
5. El sistema transmita continuamente los datos de los cuatro sensores mediante comunicación UART al computador durante al menos 2 minutos sin errores ni reinicios.
6. Se registren al menos 50 mediciones consecutivas correctas de todas las variables monitoreadas.
7. La pantalla OLED muestre los valores de temperatura, humedad del suelo, luminosidad y nivel de agua.
## 8. Métrica de Éxito

**¿Cómo se sabe, sin ambigüedad, que el proyecto funciona?**
### Nuestra métrica de éxito

✅ **El proyecto se considerará exitoso si durante una prueba continua de 2 horas el sistema mantiene la temperatura entre 22 °C y 28 °C al menos el 90% del tiempo, activa automáticamente el riego cuando la humedad del suelo cae por debajo del umbral establecido y transmite correctamente más del 95% de las mediciones de los sensores (temperatura, humedad, luminosidad y nivel de agua) a la pantalla OLED y al computador mediante UART.**

Además:

* El sensor de temperatura deberá presentar un error menor a **±1 °C** respecto a un termómetro de referencia.
* El sensor de nivel de agua deberá presentar un error menor a **±2 cm** respecto a una medición manual.
* El sistema deberá detectar correctamente el estado de tanque vacío en **10 de 10 pruebas consecutivas**.
* Los actuadores (bomba, ventilador y termoresistencia) deberán responder en menos de **3 segundos** después de que una variable exceda su umbral de activación.

Esta métrica es **cuantitativa, medible y observable**, porque puede verificarse experimentalmente y determinar claramente si se cumple o no.


## 9. Plan de Validación

**¿Cómo van a probar que su dispositivo funciona correctamente?**

- ¿Contra qué referencia validan? (valor teórico, otro instrumento, prueba con usuarios, comparación A/B)
- Si es un instrumento de medición: ¿cuántas mediciones van a tomar? (mínimo N ≥ 10)
- Si es un dispositivo interactivo: ¿cómo prueban que la experiencia funciona?
- ¿Qué análisis van a hacer? (promedio, desviación estándar, error relativo, tasa de aciertos, etc.)

---

## 10. Bitácora de Diseño

> 📓 **Obligatorio para presentar en cada entregable (no tiene nota, pero sin bitácora no hay presentación).**

La bitácora es un diario de ingeniería en Markdown que registra:

- Qué hicieron en cada sesión de trabajo
- Qué funcionó y qué falló
- Decisiones de diseño y su justificación
- Cambios respecto al plan original (y por qué)

**Formato sugerido:**

```markdown
# Bitácora — Proyecto [Nombre]

## Semana 1 — [Fecha]

**Qué hicimos:**
- Conectamos el LM35 al pin A0
- Probamos lectura básica con analogRead()

**Qué falló:**
- El sensor marcaba 10°C de más — descubrimos que era ruido de la fuente

**Qué decidimos:**
- Agregar filtro de media móvil (S8)
- Usar fuente externa en vez de USB para reducir ruido

**Plan próxima sesión:**
- Implementar filtro y verificar precisión con termómetro de referencia
```

---

## Checklist de Verificación (antes de entregar)

- [ ] ¿Cada integrante puede explicar el proyecto completo?
- [ ] ¿El diagrama de bloques muestra todos los subsistemas?
- [ ] ¿El proyecto usa al menos 4 habilidades distintas de S1-S9?
- [ ] ¿La lista de materiales incluye costo, proveedor, disponibilidad y Plan B?
- [ ] ¿El cronograma define los 3 entregables (semanas 2, 5, 7) con demos claras?
- [ ] ¿El MVP está definido? (si todo falla, ¿qué muestran?)
- [ ] ¿La métrica de éxito es cuantitativa y falsable?
- [ ] ¿El plan de validación especifica referencia y número de mediciones?
- [ ] ¿Iniciaron la bitácora de diseño?

---

> 🎯 **Recuerden:** El objetivo NO es hacer el proyecto más complejo posible, sino un proyecto **bien ejecutado, robusto y que demuestre lo que aprendieron.** Con 7 semanas, proyectos bien acotados y bien hechos brillan más que proyectos ambiciosos a medias. No importa si es un instrumento de medición, un dispositivo interactivo, o la automatización de un experimento — lo importante es que uses el toolkit S1-S9 y funcione.
