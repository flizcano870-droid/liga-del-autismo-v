#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===== PINES =====
const int TRIG_PIN = 2;
const int ECHO_PIN = 3;
const int LED1_PIN = 4; // Luz baja (LDR)
const int LED2_PIN = 5; // Falta de agua (Ultrasonido)
const int LED3_PIN = 6; // Aire malo (MQ135)

const int TEMP_PIN = 7; // Calefacción
const int PUMP_PIN = 8; // Bomba de riego
const int FAN1_PIN = 9; // Ventilador 1
const int FAN2_PIN = 10;// Ventilador 2
const int BUZZER_PIN = 11;

const int LM35_PIN = A1;
const int SOIL_PIN = A3;
const int MQ135_PIN = A0;
const int LDR_PIN = A2;

// ===== UMBRALES =====
const float NIVEL_MAX = 6.47;
const float NIVEL_MIN = 13.0;

const int SOIL_SECO = 494;
const int SOIL_HUMEDO = 421;

const int LDR_SOMBRA = 250;
const int LDR_MAX = 440;

const int MQ_MALO = 170;
const int MQ_PELIGRO = 250;

// ===== MAQUINA DE ESTADOS / LISTA DE TAREAS =====
enum TareaSensor {
  TAREA_AGUA = 0,
  TAREA_SUELO,
  TAREA_TEMP,
  TAREA_MQ,
  TAREA_LUZ
};

TareaSensor tareaActual = TAREA_AGUA;
unsigned long tiempoInicioTarea = 0;
const unsigned long DURACION_TAREA = 4000; // 4 segundos por sensor

// ===== VARIABLES PERSISTENTES DE SENSORES Y ACTUADORES =====
float distanciaGuardada = 10.0;
int aguaPctGuardado = 50;
bool tanqueVacio = false;

int sueloRawGuardado = 450;
int sueloPctGuardado = 50;

float tempGuardada = 20.0;
bool ventiladorPorTemp = false;

int mqRawGuardado = 100;
int mqPctGuardado = 0;
bool ventiladorPorMQ = false;

int ldrRawGuardado = 300;
int luzPctGuardado = 50;

bool buzzerAgua = false;
bool buzzerMQ = false;

// Acumuladores de muestras dentro de cada intervalo de 4s
float sumaLecturas = 0;
int cantidadMuestras = 0;
unsigned long ultimoMuestreo = 0;

// ===== ESTRUCTURA PARA BEAT NO BLOQUEANTE EN BUZZER =====
struct PasoBeat {
  int frecuencia; // 0 = silencio
  unsigned long duracionMs;
};

// Beat de Reggaetón para MQ-135 > 200: TUM - ta - TUM - ta
PasoBeat beatReggaeton[] = {
  {220, 180},
  {0,    80},
  {440, 120},
  {0,    80},
  {220, 180},
  {0,    80},
  {440, 120},
  {0,   250}
};
const int TOTAL_PASOS_REGGAETON = 8;

// Pitido de falta de agua
PasoBeat beatAgua[] = {
  {500, 150},
  {0,   150}
};
const int TOTAL_PASOS_AGUA = 2;

int pasoBeatActual = 0;
unsigned long ultimoCambioPasoBeat = 0;

// ===== FUNCIONES AUXILIARES DE CÁLCULO =====
float medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duracion = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duracion == 0) return -1;

  return duracion * 0.0343 / 2.0;
}

int calcularPorcentajeAgua(float distancia) {
  int porcentaje = map(distancia * 100, NIVEL_MIN * 100, NIVEL_MAX * 100, 0, 100);
  return constrain(porcentaje, 0, 100);
}

int calcularPorcentajeSuelo(int raw) {
  int porcentaje = map(raw, SOIL_SECO, SOIL_HUMEDO, 0, 100);
  return constrain(porcentaje, 0, 100);
}

int calcularPorcentajeLuz(int raw) {
  int porcentaje = map(raw, 210, LDR_MAX, 0, 100);
  return constrain(porcentaje, 0, 100);
}

int calcularPorcentajeMQ(int raw) {
  int porcentaje = map(raw, MQ_MALO, MQ_PELIGRO, 0, 100);
  return constrain(porcentaje, 0, 100);
}

float leerTemperatura() {
  int lectura = analogRead(LM35_PIN);
  float voltaje = lectura * (5.0 / 1023.0);
  return voltaje * 100.0;
}

// ===== GESTIÓN NO BLOQUEANTE DEL BUZZER =====
void actualizarBuzzer(unsigned long tiempoActual) {
  if (buzzerMQ) {
    // Alarma MQ-135 con beat de Reggaetón
    PasoBeat paso = beatReggaeton[pasoBeatActual];
    if (tiempoActual - ultimoCambioPasoBeat >= paso.duracionMs) {
      ultimoCambioPasoBeat = tiempoActual;
      pasoBeatActual = (pasoBeatActual + 1) % TOTAL_PASOS_REGGAETON;
      
      if (beatReggaeton[pasoBeatActual].frecuencia > 0) {
        tone(BUZZER_PIN, beatReggaeton[pasoBeatActual].frecuencia);
      } else {
        noTone(BUZZER_PIN);
      }
    }
  } 
  else if (buzzerAgua) {
    // Alarma de Falta de Agua
    PasoBeat paso = beatAgua[pasoBeatActual];
    if (tiempoActual - ultimoCambioPasoBeat >= paso.duracionMs) {
      ultimoCambioPasoBeat = tiempoActual;
      pasoBeatActual = (pasoBeatActual + 1) % TOTAL_PASOS_AGUA;
      
      if (beatAgua[pasoBeatActual].frecuencia > 0) {
        tone(BUZZER_PIN, beatAgua[pasoBeatActual].frecuencia);
      } else {
        noTone(BUZZER_PIN);
      }
    }
  } 
  else {
    noTone(BUZZER_PIN);
    pasoBeatActual = 0;
  }
}

// ===== PANTALLA OLED =====
void actualizarOLED() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  // Fila 0: Indicador de la tarea que se está midiendo en este momento
  display.setCursor(0, 0);
  display.print("[");
  switch (tareaActual) {
    case TAREA_AGUA:  display.print("1/5 MIDE: AGUA"); break;
    case TAREA_SUELO: display.print("2/5 MIDE: SUELO"); break;
    case TAREA_TEMP:  display.print("3/5 MIDE: TEMP"); break;
    case TAREA_MQ:    display.print("4/5 MIDE: AIRE"); break;
    case TAREA_LUZ:   display.print("5/5 MIDE: LUZ"); break;
  }
  display.print("]");

  // Fila 1: Valores registrados
  display.setCursor(0, 10);
  display.print("T:");
  display.print(tempGuardada, 1);
  display.print("C  S:");
  display.print(sueloPctGuardado);
  display.print("%");

  // Fila 2: Aire y Agua
  display.setCursor(0, 18);
  display.print("MQ:");
  display.print(mqRawGuardado);
  display.print("  Agua:");
  display.print(aguaPctGuardado);
  display.print("%");

  // Fila 3: Luz y Estado general
  display.setCursor(0, 26);
  display.print("Luz:");
  display.print(luzPctGuardado);
  display.print("%  ");
  if (tanqueVacio) display.print("!VACIO!");
  else if (buzzerMQ) display.print("!GAS!");
  else display.print("OK");

  display.display();
}

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  pinMode(TEMP_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(FAN1_PIN, OUTPUT);
  pinMode(FAN2_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  digitalWrite(TEMP_PIN, LOW);
  digitalWrite(PUMP_PIN, LOW);
  digitalWrite(FAN1_PIN, LOW);
  digitalWrite(FAN2_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }

  tiempoInicioTarea = millis();
}

void loop() {
  unsigned long tiempoActual = millis();

  // ========================================================
  // 1. TOMA DE MUESTRAS DEL SENSOR ACTUAL (CADA 100 ms)
  // ========================================================
  if (tiempoActual - ultimoMuestreo >= 100) {
    ultimoMuestreo = tiempoActual;

    switch (tareaActual) {
      case TAREA_AGUA: {
        float d = medirDistancia();
        if (d >= 0) {
          sumaLecturas += d;
          cantidadMuestras++;
        }
        break;
      }
      case TAREA_SUELO:
        sumaLecturas += analogRead(SOIL_PIN);
        cantidadMuestras++;
        break;

      case TAREA_TEMP:
        sumaLecturas += leerTemperatura();
        cantidadMuestras++;
        break;

      case TAREA_MQ:
        sumaLecturas += analogRead(MQ135_PIN);
        cantidadMuestras++;
        break;

      case TAREA_LUZ:
        sumaLecturas += analogRead(LDR_PIN);
        cantidadMuestras++;
        break;
    }
  }

  // ========================================================
  // 2. CAMBIO DE TAREA (CADA 4 SEGUNDOS) Y ACTUALIZACIÓN
  // ========================================================
  if (tiempoActual - tiempoInicioTarea >= DURACION_TAREA) {
    
    // Calcular promedio final del sensor si hubo muestras
    float promedio = (cantidadMuestras > 0) ? (sumaLecturas / cantidadMuestras) : 0;

    // Procesar según la tarea que acaba de terminar y dejar actuadores guardados
    switch (tareaActual) {

      case TAREA_AGUA:
        if (cantidadMuestras > 0) {
          distanciaGuardada = promedio;
          aguaPctGuardado = calcularPorcentajeAgua(distanciaGuardada);
          tanqueVacio = (distanciaGuardada >= NIVEL_MIN);

          // Actuadores de Agua (Persisten hasta que vuelva a tocar medir Agua)
          digitalWrite(LED2_PIN, tanqueVacio);
          buzzerAgua = tanqueVacio;

          // Si se vació el tanque, la bomba debe apagarse de inmediato por seguridad
          if (tanqueVacio) {
            digitalWrite(PUMP_PIN, LOW);
          }
        }
        break;

      case TAREA_SUELO:
        if (cantidadMuestras > 0) {
          sueloRawGuardado = (int)promedio;
          sueloPctGuardado = calcularPorcentajeSuelo(sueloRawGuardado);

          // Actuador de Bomba (Persiste hasta la siguiente medición de Suelo)
          if (sueloRawGuardado >= SOIL_SECO && !tanqueVacio) {
            digitalWrite(PUMP_PIN, HIGH);
          } else if (sueloRawGuardado <= SOIL_HUMEDO || tanqueVacio) {
            digitalWrite(PUMP_PIN, LOW);
          }
        }
        break;

      case TAREA_TEMP:
        if (cantidadMuestras > 0) {
          tempGuardada = promedio;

          // Calefacción (Persiste hasta la siguiente medición de Temp)
          digitalWrite(TEMP_PIN, (tempGuardada < 12.0));

          // Evaluador de Ventiladores por temperatura
          ventiladorPorTemp = (tempGuardada > 25.0);

          // Aplica el estado combinado a ventiladores
          bool estadoVentiladores = (ventiladorPorTemp || ventiladorPorMQ);
          digitalWrite(FAN1_PIN, estadoVentiladores);
          digitalWrite(FAN2_PIN, estadoVentiladores);
        }
        break;

      case TAREA_MQ:
        if (cantidadMuestras > 0) {
          mqRawGuardado = (int)promedio;
          mqPctGuardado = calcularPorcentajeMQ(mqRawGuardado);

          bool aireMalo = (mqRawGuardado > 200);
          digitalWrite(LED3_PIN, aireMalo);
          buzzerMQ = aireMalo;

          // Evaluador de Ventiladores por MQ135
          ventiladorPorMQ = aireMalo;

          // Aplica el estado combinado a ventiladores
          bool estadoVentiladores = (ventiladorPorTemp || ventiladorPorMQ);
          digitalWrite(FAN1_PIN, estadoVentiladores);
          digitalWrite(FAN2_PIN, estadoVentiladores);
        }
        break;

      case TAREA_LUZ:
        if (cantidadMuestras > 0) {
          ldrRawGuardado = (int)promedio;
          luzPctGuardado = calcularPorcentajeLuz(ldrRawGuardado);

          // LED1 Alarma Luz Baja (Persiste hasta la siguiente medición de Luz)
          digitalWrite(LED1_PIN, (ldrRawGuardado < LDR_SOMBRA));
        }
        break;
    }

    // Imprimir reporte Serial al terminar cada tarea
    Serial.print("Tarea Finalizada: ");
    Serial.print(tareaActual);
    Serial.print(" | Temp: "); Serial.print(tempGuardada, 1);
    Serial.print("C | Suelo: "); Serial.print(sueloPctGuardado);
    Serial.print("% | MQ: "); Serial.print(mqRawGuardado);
    Serial.print(" | Luz: "); Serial.print(luzPctGuardado);
    Serial.print("% | Agua: "); Serial.print(aguaPctGuardado);
    Serial.println("%");

    // Pasar a la siguiente tarea en la lista (0 -> 1 -> 2 -> 3 -> 4 -> 0...)
    tareaActual = (TareaSensor)((tareaActual + 1) % 5);
    
    // Reiniciar contadores para los siguientes 4 segundos
    sumaLecturas = 0;
    cantidadMuestras = 0;
    tiempoInicioTarea = tiempoActual;
  }

  // ========================================================
  // 3. TAREAS DE FONDO CONTINUAS (BUZZER Y PANTALLA)
  // ========================================================
  actualizarBuzzer(tiempoActual);
  actualizarOLED();
}