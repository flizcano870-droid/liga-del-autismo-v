#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pines
const int TRIG_PIN = 2;
const int ECHO_PIN = 3;
const int LED1_PIN = 4; // Luz baja
const int LED2_PIN = 5; // Falta de agua
const int LED3_PIN = 6; // Aire malo

const int TEMP_PIN = 7; // Calefacción
const int PUMP_PIN = 8; // Bomba
const int FAN1_PIN = 9;
const int FAN2_PIN = 10;
const int BUZZER_PIN = 11;

const int LM35_PIN = A1;
const int SOIL_PIN = A3;
const int MQ135_PIN = A0;
const int LDR_PIN = A2;

// Umbrales
const float NIVEL_MAX = 6.47;
const float NIVEL_MIN = 13.0;

const int SOIL_SECO = 494;
const int SOIL_HUMEDO = 421;

const int LDR_SOMBRA = 250;
const int LDR_MAX = 440;

const int MQ_MALO = 170;
const int MQ_PELIGRO = 250;

bool buzzerAgua = false;
bool buzzerMQ = false;

unsigned long ultimoCambioBuzzer = 0;
bool estadoBuzzer = false;

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
  int porcentaje = map(distancia * 100,
                       NIVEL_MIN * 100,
                       NIVEL_MAX * 100,
                       0, 100);
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
  digitalWrite(BUZZER_PIN, LOW);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }

  display.clearDisplay();
  display.display();
}

void loop() {

  // =========================
  // PROMEDIO DURANTE 10 SEGUNDOS
  // =========================
  float sumaTemp = 0;
  float sumaDist = 0;
  long sumaSuelo = 0;
  long sumaMQ = 0;
  long sumaLDR = 0;

  const int muestras = 50; // 50 muestras en 10 segundos (cada 200 ms)

  for (int i = 0; i < muestras; i++) {
    sumaTemp += leerTemperatura();
    sumaDist += medirDistancia();
    sumaSuelo += analogRead(SOIL_PIN);
    sumaMQ += analogRead(MQ135_PIN);
    sumaLDR += analogRead(LDR_PIN);

    delay(200); // 50 x 200 ms = 10 s
  }

  // Valores promediados
  float temperatura = (sumaTemp / muestras);
  float distancia = sumaDist / muestras;
  int sueloRaw = sumaSuelo / muestras;
  int mqRaw = sumaMQ / muestras;
  int ldrRaw = sumaLDR / muestras;

  // Porcentajes
  int aguaPct = calcularPorcentajeAgua(distancia);
  int sueloPct = calcularPorcentajeSuelo(sueloRaw);
  int luzPct = calcularPorcentajeLuz(ldrRaw);
  int mqPct = calcularPorcentajeMQ(mqRaw);

  // =========================
  // ACTUADORES Y LEDS
  // =========================

  // Luz baja
  digitalWrite(LED1_PIN, (ldrRaw < LDR_SOMBRA));

  // Nivel de agua bajo
  bool tanqueVacio = (distancia >= NIVEL_MIN);
  digitalWrite(LED2_PIN, tanqueVacio);
  buzzerAgua = tanqueVacio;

  // Calidad del aire
  bool aireMalo = (mqRaw > 200); // ahora solo por encima de 200
  digitalWrite(LED3_PIN, aireMalo);
  buzzerMQ = aireMalo;

  // Calefacción
  digitalWrite(TEMP_PIN, (temperatura < 12.0));

  // Ventiladores: SOLO si temperatura muy alta o MQ > 200
  bool ventiladorON = (temperatura > 25.0 || mqRaw > 200);
  digitalWrite(FAN1_PIN, ventiladorON);
  digitalWrite(FAN2_PIN, ventiladorON);

  // Bomba de riego
  if (sueloRaw >= SOIL_SECO && !tanqueVacio) {
    digitalWrite(PUMP_PIN, HIGH);
  } else if (sueloRaw <= SOIL_HUMEDO || tanqueVacio) {
    digitalWrite(PUMP_PIN, LOW);
  }

  // =========================
  // BUZZER CON DOS RITMOS
  // =========================
  unsigned long tiempoActual = millis();

  // =========================
// BUZZER CON BEAT DE REGGAETON
// =========================
if (buzzerMQ) {

  // Beat de reggaetón: TUM - ta - TUM - ta
  tone(BUZZER_PIN, 220); // TUM grave
  delay(180);
  noTone(BUZZER_PIN);
  delay(80);

  tone(BUZZER_PIN, 440); // ta agudo
  delay(120);
  noTone(BUZZER_PIN);
  delay(80);

  tone(BUZZER_PIN, 220); // TUM grave
  delay(180);
  noTone(BUZZER_PIN);
  delay(80);

  tone(BUZZER_PIN, 440); // ta agudo
  delay(120);
  noTone(BUZZER_PIN);
  delay(250); // pausa para repetir el beat
}

else if (buzzerAgua) {

  // Falta de agua: pitido normal y más lento
  tone(BUZZER_PIN, 500);
  delay(150);
  noTone(BUZZER_PIN);
  delay(150);
}

else {
  noTone(BUZZER_PIN);
}

  // =========================
  // OLED
  // =========================
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.print("Temp:");
  display.print(temperatura, 1);
  display.println("C");

  display.setCursor(0, 8);
  display.print("Suelo:");
  display.print(sueloPct);
  display.print("%");

  display.setCursor(64, 8);
  display.print("Luz:");
  display.print(luzPct);
  display.print("%");

  display.setCursor(0, 16);
  display.print("MQ:");
  display.print(mqRaw);
  display.print(" ");
  display.print(mqPct);
  display.print("%");

  display.setCursor(0, 24);
  display.print("Agua:");
  display.print(aguaPct);
  display.print("%");

  display.display();

  // =========================
  // SERIAL
  // =========================
  Serial.print("Temp: ");
  Serial.print(temperatura, 1);
  Serial.print(" C | Suelo: ");
  Serial.print(sueloPct);
  Serial.print("% | MQ: ");
  Serial.print(mqRaw);
  Serial.print(" (");
  Serial.print(mqPct);
  Serial.print("%) | Luz: ");
  Serial.print(luzPct);
  Serial.print("% | Agua: ");
  Serial.print(aguaPct);
  Serial.println("%");
}