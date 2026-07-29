#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===== PINES =====
const int TRIG_PIN = 2;
const int ECHO_PIN = 3;
const int LED1_PIN = 4; // Luz de apoyo (LDR)
const int LED2_PIN = 5; // Falta de agua en tanque (Ultrasonido)
const int LED3_PIN = 6; // Aire malo (MQ135)

const int TEMP_PIN = 7;  // Calefacción
const int PUMP_PIN = 8;  // Bomba de riego
const int FAN1_PIN = 10; // Ventilador 1
const int FAN2_PIN = 9;  // Ventilador 2
const int BUZZER_PIN = 11;

const int LM35_PIN = A1;
const int SOIL_PIN = A3;
const int MQ135_PIN = A0;
const int LDR_PIN = A2;

// ===== UMBRALES =====
const float NIVEL_MAX = 6.47;  // Distancia con tanque lleno (cm)
const float NIVEL_MIN = 13.0;  // Distancia con tanque vacío (cm)

const int SOIL_SECO = 494;   // Lectura ADC que representa 0% de humedad
const int SOIL_HUMEDO = 421; // Lectura ADC que representa 100% de humedad

const int LDR_SOMBRA = 250;  // Umbral de oscuridad/sombra
const int LDR_MAX = 440;

const int MQ_MALO = 170;
const int MQ_PELIGRO = 250;

// ===== TIEMPOS DE RIEGO POR PULSOS =====
const unsigned long DURACION_RIEGO = 10000;  // 10 segundos de riego activado
const unsigned long COOLDOWN_RIEGO = 300000; // 5 minutos de reposo/absorción (300.000 ms)

bool bombaActiva = false;
unsigned long tiempoInicioBomba = 0;
unsigned long tiempoFinBomba = 0;
bool primeraVezRiego = true;

// ===== MÁQUINA DE ESTADOS / ROTACIÓN DE SENSORES =====
enum TareaSensor {
  TAREA_AGUA = 0,
  TAREA_SUELO,
  TAREA_TEMP,
  TAREA_MQ,
  TAREA_LUZ
};

TareaSensor tareaActual = TAREA_AGUA;
unsigned long tiempoInicioTarea = 0;
const unsigned long DURACION_TAREA = 5000; // 5 segundos por cada sensor

// ===== VARIABLES PERSISTENTES DE LECTURA =====
float distanciaGuardada = 10.0;
int aguaPctGuardado = 50;
bool tanqueVacio = false;

int sueloRawGuardado = 450;
int sueloPctGuardado = 50;

float tempGuardada = 20.0;
bool ventiladorPorTemp = false;

int mqRawGuardado = 100;
bool ventiladorPorMQ = false;

int ldrRawGuardado = 300;
int luzPctGuardado = 50;

bool buzzerAgua = false;
bool buzzerMQ = false;

// Acumuladores de muestras
float sumaLecturas = 0;
int cantidadMuestras = 0;
unsigned long ultimoMuestreo = 0;

// ===== ESTRUCTURA Y PATRONES DEL BUZZER =====
struct PasoBeat {
  int frecuencia; // 0 = silencio
  unsigned long duracionMs;
};

// Beat de alerta para calidad de aire (MQ-135 > 200)
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

// Pitido de alerta para tanque vacío
PasoBeat beatAgua[] = {
  {500, 150},
  {0,   150}
};
const int TOTAL_PASOS_AGUA = 2;

int pasoBeatActual = 0;
unsigned long ultimoCambioPasoBeat = 0;

// ===== LECTURA ANTI-RUIDO (DESCARGA DE RESIDUO EN ADC) =====
int analogReadLimpio(int pin) {
  analogRead(pin);          // Descarta la primera lectura residual del multiplexor
  delayMicroseconds(150);   // Tiempo de estabilización
  return analogRead(pin);   // Lectura real estable
}

// ===== FUNCIONES AUXILIARES DE CÁLCULO =====
float medirDistancia() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duracion = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout a 30ms
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

float leerTemperatura() {
  int lectura = analogReadLimpio(LM35_PIN);
  float voltaje = lectura * (5.0 / 1023.0);
  return voltaje * 100.0;
}

// ===== CONTROL NO BLOQUEANTE DE LA BOMBA DE RIEGO =====
void actualizarBomba(unsigned long tiempoActual) {
  if (bombaActiva) {
    // Apaga la bomba tras cumplir 10 segundos O de inmediato si el tanque se vacía
    if (tiempoActual - tiempoInicioBomba >= DURACION_RIEGO || tanqueVacio) {
      digitalWrite(PUMP_PIN, LOW);
      bombaActiva = false;
      tiempoFinBomba = tiempoActual; // Marca el tiempo inicial de los 5 min de descanso
      primeraVezRiego = false;
    }
  }
}

// ===== CONTROL NO BLOQUEANTE DEL BUZZER =====
void actualizarBuzzer(unsigned long tiempoActual) {
  if (buzzerMQ) {
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
  display.setTextSize(2);  
  display.setCursor(0, 8); 

  switch (tareaActual) {

    case TAREA_AGUA:
      display.print("Agua: ");
      display.print(aguaPctGuardado);
      display.print("%");
      break;

    case TAREA_SUELO:
      display.print("Hum: ");
      display.print(sueloPctGuardado);
      display.print("%");
      break;

    case TAREA_TEMP:
      display.print("Temp:");
      display.print(tempGuardada, 1);
      display.print("C");
      break;

    case TAREA_MQ:
      display.print("Aire:");
      if (mqRawGuardado < 170) {
        display.print(" OK");
      } else if (mqRawGuardado <= 200) {
        display.print("Medio");
      } else {
        display.print(" Malo");
      }
      break;

    case TAREA_LUZ:
      display.print("Luz: ");
      display.print(luzPctGuardado);
      display.print("%");
      break;
  }

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
    while (1); // Detiene el programa si la pantalla no responde
  }

  tiempoInicioTarea = millis();
}

void loop() {
  unsigned long tiempoActual = millis();

  // ========================================================
  // 1. MUESTREO DEL SENSOR ACTIVO (CADA 100 ms)
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
        sumaLecturas += analogReadLimpio(SOIL_PIN);
        cantidadMuestras++;
        break;

      case TAREA_TEMP:
        sumaLecturas += leerTemperatura();
        cantidadMuestras++;
        break;

      case TAREA_MQ:
        sumaLecturas += analogReadLimpio(MQ135_PIN);
        cantidadMuestras++;
        break;

      case TAREA_LUZ:
        sumaLecturas += analogReadLimpio(LDR_PIN);
        cantidadMuestras++;
        break;
    }
  }

  // ========================================================
  // 2. CAMBIO DE SENSOR Y LÓGICA DE CONTROL (CADA 5 SEGUNDOS)
  // ========================================================
  if (tiempoActual - tiempoInicioTarea >= DURACION_TAREA) {
    
    float promedio = (cantidadMuestras > 0) ? (sumaLecturas / cantidadMuestras) : 0;

    switch (tareaActual) {

      case TAREA_AGUA:
        if (cantidadMuestras > 0) {
          distanciaGuardada = promedio;
          aguaPctGuardado = calcularPorcentajeAgua(distanciaGuardada);
          tanqueVacio = (distanciaGuardada >= NIVEL_MIN);

          digitalWrite(LED2_PIN, tanqueVacio);
          buzzerAgua = tanqueVacio;
        }
        break;

      case TAREA_SUELO:
        if (cantidadMuestras > 0) {
          sueloRawGuardado = (int)promedio;
          sueloPctGuardado = calcularPorcentajeSuelo(sueloRawGuardado);

          // Valida si transcurrieron los 5 minutos de reposo tras el último riego
          bool cooldownSuperado = primeraVezRiego || (tiempoActual - tiempoFinBomba >= COOLDOWN_RIEGO);

          // Si humedad < 60%, hay agua y pasaron los 5 min, arranca pulso de 10s
          if (sueloPctGuardado < 60 && !tanqueVacio && !bombaActiva && cooldownSuperado) {
            digitalWrite(PUMP_PIN, HIGH);
            bombaActiva = true;
            tiempoInicioBomba = tiempoActual;
          }
        }
        break;

      case TAREA_TEMP:
        if (cantidadMuestras > 0) {
          tempGuardada = promedio;

          digitalWrite(TEMP_PIN, (tempGuardada < 12.0)); // Calefacción si < 12°C
          ventiladorPorTemp = (tempGuardada > 25.0);    // Ventilación si > 25°C
        }
        break;

      case TAREA_MQ:
        if (cantidadMuestras > 0) {
          mqRawGuardado = (int)promedio;

          bool aireMalo = (mqRawGuardado > 200);
          digitalWrite(LED3_PIN, aireMalo);
          buzzerMQ = aireMalo;
          ventiladorPorMQ = aireMalo;
        }
        break;

      case TAREA_LUZ:
        if (cantidadMuestras > 0) {
          ldrRawGuardado = (int)promedio;
          luzPctGuardado = calcularPorcentajeLuz(ldrRawGuardado);

          digitalWrite(LED1_PIN, (ldrRawGuardado < LDR_SOMBRA)); // Luz si hay sombra/oscuridad
        }
        break;
    }

    // Control de ventiladores (se activan si lo pide la temperatura O la calidad de aire)
    bool estadoVentiladores = (ventiladorPorTemp || ventiladorPorMQ);
    digitalWrite(FAN1_PIN, estadoVentiladores);
    digitalWrite(FAN2_PIN, estadoVentiladores);

    // Monitoreo por puerto Serial (útil para depuración)
    Serial.print("Sensor "); Serial.print(tareaActual + 1);
    Serial.print("/5 | Temp: "); Serial.print(tempGuardada, 1);
    Serial.print("C | Hum.Suelo: "); Serial.print(sueloPctGuardado);
    Serial.print("% (Raw: "); Serial.print(sueloRawGuardado);
    Serial.print(") | MQ: "); Serial.print(mqRawGuardado);
    Serial.print(" | Luz: "); Serial.print(luzPctGuardado);
    Serial.print("% | Agua: "); Serial.print(aguaPctGuardado);
    Serial.println("%");

    // Siguiente sensor en la ronda
    tareaActual = (TareaSensor)((tareaActual + 1) % 5);
    
    // Reiniciar contadores para la siguiente ventana de 5s
    sumaLecturas = 0;
    cantidadMuestras = 0;
    tiempoInicioTarea = tiempoActual;
  }

  // ========================================================
  // 3. EJECUCIÓN CONTINUA (NO BLOQUEANTE)
  // ========================================================
  actualizarBomba(tiempoActual);
  actualizarBuzzer(tiempoActual);
  actualizarOLED();
}