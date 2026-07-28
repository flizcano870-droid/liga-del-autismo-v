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

    int soilValue = analogRead(SOIL_PIN);

    // LDR
    int ldrValue  = analogRead(LDR_PIN);
    bool esDia = (ldrValue > LDR_DIA);

    // TEMPERATURA
    // LM35
    int lm35Raw = analogRead(LM35_PIN);
    float voltaje = lm35Raw * (5.0 / 1023.0);
    float temperatura = voltaje * 100.0;

    temperaturas[indiceTemp] = temperatura;

    indiceTemp++;

    if (indiceTemp >= NUM_MUESTRAS) {
      indiceTemp = 0;
      bufferLleno = true;
    }

    float temperaturaProm = 0;

    int cantidad = bufferLleno ? NUM_MUESTRAS : indiceTemp;

    for (int i = 0; i < cantidad; i++) {
      temperaturaProm += temperaturas[i];
    }

    temperaturaProm /= cantidad;

    float tempMin;
    float tempMax;
    if (esDia) {
      tempMin = TEMP_MIN_DIA;
      tempMax = TEMP_MAX_DIA;
    }
    else {
      tempMin = TEMP_MIN_NOCHE;
      tempMax = TEMP_MAX_NOCHE;
    }

        // ===== Ventilación =====
    if (!ventiladoresEncendidos && temperaturaProm > tempMax + HISTERESIS) {

      ventiladoresEncendidos = true;

    }
    else if (ventiladoresEncendidos && temperaturaProm < tempMax) {

      ventiladoresEncendidos = false;

    }

    // ===== Calefacción =====
    if (!calefactorEncendido && temperaturaProm < tempMin - HISTERESIS) {

      calefactorEncendido = true;

    }
    else if (calefactorEncendido && temperaturaProm > tempMin) {

      calefactorEncendido = false;

    }

    // Aplicar estados
    digitalWrite(FAN1_PIN, ventiladoresEncendidos);
    digitalWrite(FAN2_PIN, ventiladoresEncendidos);
    digitalWrite(TEMP_PIN, calefactorEncendido);

    // CALIDAD DEL AIRE
    int mqValue   = analogRead(MQ_PIN); 
    unsigned long tiempoActual = millis();

    if (mqValue >= MQ_PELIGRO) {

      digitalWrite(LED3_PIN, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);

    }
    else if (mqValue >= MQ_ALERTA) {

      digitalWrite(LED3_PIN, HIGH);

      if (tiempoActual - tiempoAnteriorBuzzer >= 250) {

        tiempoAnteriorBuzzer = tiempoActual;

        estadoBuzzer = !estadoBuzzer;

        digitalWrite(BUZZER_PIN, estadoBuzzer);

      }

    }
    else {

      digitalWrite(LED3_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      estadoBuzzer = false;

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