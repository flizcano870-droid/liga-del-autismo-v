# Propuesta de Proyecto Final — Electrónica Digital 2026-1

**Grupo #:** ___  
**Integrantes:**  
1. ___  
2. ___  
3. ___  
4. ___  

**Fecha de entrega:** ___

---

> ⚠️ **Regla de oro:** Cada integrante debe poder explicar y defender cada decisión de diseño y cada línea de código. En cada entregable y en la presentación final se hará una pregunta aleatoria a cualquier miembro del grupo.

---

## 1. Descripción del Proyecto

**¿Qué van a construir?**

Describan el dispositivo, instrumento o sistema que van a diseñar y fabricar. Puede ser:

- La automatización de un experimento de física
- Un instrumento de medición
- Un dispositivo interactivo (instrumento musical, controlador, juego electrónico)
- Un sistema de control o monitoreo
- Cualquier proyecto que aproveche el toolkit del curso

Incluyan:

- ¿Qué hace su dispositivo? ¿Qué problema resuelve o qué necesidad satisface?
- ¿Quién lo usaría y para qué?
- ¿Qué entradas recibe y qué salidas produce? (señales, movimiento, sonido, luz, datos, etc.)
- **Describan una sesión típica de uso:** desde que el usuario enciende el dispositivo hasta que obtiene un resultado. ¿Qué ve, qué escucha, qué hace?

---

## 2. Solución Propuesta — Diagrama de Bloques

**¿Cómo resuelven el problema con el toolkit del curso?**

Dibujen un diagrama de bloques del sistema completo. Cada bloque debe ser un subsistema identificable con su función específica.

La arquitectura depende del tipo de proyecto. Algunos ejemplos:

**Tipo A — Instrumento de medición / automatización:**
```
┌──────────┐    ┌──────────┐    ┌──────────┐
│ SENSORES │ →  │ ARDUINO  │ →  │ACTUADORES│
└──────────┘    └──────────┘    └──────────┘
                      │
               ┌──────┴──────┐
               │  DISPLAY /  │
               │  PC / LOG   │
               └─────────────┘
```

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

| Habilidad del curso | Subsistema 1: (nombre) | Subsistema 2: (nombre) | Subsistema 3: (nombre) |
|---|---|---|---|
| **S1** — E/S digital, protoboard | | | |
| **S2** — Timing preciso, interrupciones | | | |
| **S3** — Debouncing, ADC, sensores analógicos | | | |
| **S4** — Comunicación UART + Python | | | |
| **S5** — PWM, H-Bridge, actuadores | | | |
| **S6** — Adquisición multicanal, OLED I2C | | | |
| **S7** — Control PID | | | |
| **S8** — Filtrado digital, oversampling, triggers | | | |
| **S9** — DAC MCP4725, FSM, generación de señales | | | |

> Si el proyecto completo usa menos de 4 habilidades distintas del curso, probablemente es demasiado simple. Pero no inflen la matriz: solo marquen las habilidades que REALMENTE usan.

---

## 4. Diseño Técnico

### 4.1 Diagrama Esquemático

Dibujen el esquemático completo mostrando:
- Todos los pines de Arduino utilizados
- Conexiones de sensores, actuadores, display, DAC
- Resistencias, capacitores y componentes de protección
- Alimentación (5V, 12V, GND)

*Pueden dibujarlo a mano (foto) o usar Fritzing/Tinkercad.*

### 4.2 Tabla de Pines

| Pin Arduino | Conectado a | Función |
|---|---|---|
| D2 | Botón 1 (INPUT_PULLUP, LOW al presionar) | Cambio de modo |
| ... | ... | ... |

### 4.3 Arquitectura de Software

Describan la máquina de estados (FSM) de su sistema. La FSM es la implementación concreta de S9 que ya identificaron en la matriz de trazabilidad (Sección 3). Aquí la detallan con diagrama y pseudocódigo:

```
┌─────────┐   botón   ┌─────────┐
│ ESPERA  │ ────────→ │ MIDIENDO │
└─────────┘           └─────────┘
                           │
                     timeout │
                           ↓
                      ┌─────────┐
                      │ MUESTRA │
                      └─────────┘
```

Incluyan pseudocódigo de la FSM principal y las funciones clave:

```cpp
// Pseudocódigo — FSM principal
enum Estado {ESPERA, MIDIENDO, MUESTRA, CALIBRANDO};
Estado estado = ESPERA;

void loop() {
  switch(estado) {
    case ESPERA:
      // Esperar trigger del sensor
      // Transición → MIDIENDO
      break;
    case MIDIENDO:
      // Adquirir datos de sensores
      // Si completado → MUESTRA
      break;
    case MUESTRA:
      // Mostrar en OLED + enviar CSV
      // Si botón presionado → ESPERA
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
| Sensor LM35 | 1 | $1.50 | $1.50 | Cra 9 | ✅ | Termistor NTC + calibración manual |
| ... | | | | | | |

**Costo total estimado:** $___

> ⚠️ **Crítico:** Si un componente es indispensable y no se consigue, el proyecto está en riesgo. Definan un Plan B para cada componente crítico.

---

## 6. Cronograma — Entregables

*7 semanas de ejecución. La Semana 1 empieza el mismo día en que se aprueba la propuesta. Tres entregables obligatorios. Cada uno debe ser una demo funcionando, no un documento.*

| Entregable | Semana | ¿Qué se demuestra? | Pre-requisitos | Definición de "terminado" |
|:---:|:---:|---|---|---|
| **Entregable 1** | Semana 2 | ___ | ___ | ___ |
| **Entregable 2** | Semana 5 | ___ | ___ | ___ |
| **Entregable 3 (Final)** | Semana 7 | Sistema completo funcionando + defensa | ___ | ___ |

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

*Ejemplo: "Aunque no funcione el control PID completo, al menos mostraremos el sensor midiendo temperatura con logging CSV y visualización en OLED."*

**Nuestro MVP es:**

___

**Prueba de aceptación del MVP:** ¿Qué evidencia concreta demostraría que el MVP está completo?

*Ejemplo: "El sensor LM35 muestra temperatura en el OLED con actualización cada 500 ms, y el script Python guarda un CSV con al menos 100 filas de datos correctos."*

___

---

## 8. Métrica de Éxito

**¿Cómo se sabe, sin ambigüedad, que el proyecto funciona?**

Definan al menos UNA métrica cuantitativa y falsable:

*Ejemplo malo:* ❌ "El sistema mide temperatura."  
*Ejemplo bueno:* ✅ "El sistema mide temperatura con error < ±1°C respecto a un termómetro de referencia, para temperaturas entre 20°C y 60°C."

**Nuestra métrica de éxito:**

___

---

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
