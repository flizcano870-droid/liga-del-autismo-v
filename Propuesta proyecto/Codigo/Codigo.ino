#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pines
const int LM35_PIN = A1;
const int SOIL_PIN = A3;
const int MQ_PIN   = A0;
const int LDR_PIN  = A2;


const int LED1_PIN = 4;
const int LED2_PIN = 5;
const int LED3_PIN = 6;

const int FAN_PIN = 9;
const int TRIG_PIN = 3;
const int ECHO_PIN = 2;
const int PUMP_PIN = 10;
const int BUZZER_PIN = 11;

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

void setup() {

  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(FAN_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);
  digitalWrite(PUMP_PIN, LOW);
  

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error OLED");
    while (1);
  }

  display.clearDisplay();
  display.display();
  
}

void loop() {

  // LM35
  int lm35Raw = analogRead(LM35_PIN);
  float voltaje = lm35Raw * (5.0 / 1023.0);
  float temperatura = voltaje * 100.0;

  // Sensores analogicos
  int soilValue = analogRead(SOIL_PIN);
  int mqValue   = analogRead(MQ_PIN);
  int ldrValue  = analogRead(LDR_PIN);

  // Ultrasonico
  float distancia = medirDistancia();

  // Monitor serial
  Serial.print("Temp: ");
  Serial.print(temperatura);
  Serial.print(" C  ");

  Serial.print("Suelo: ");
  Serial.print(soilValue);
  Serial.print("  ");

  Serial.print("MQ: ");
  Serial.print(mqValue);
  Serial.print("  ");

  Serial.print("LDR: ");
  Serial.print(ldrValue);
  Serial.print("  ");

  Serial.print("Dist: ");
  Serial.print(distancia);
  Serial.println(" cm");

  // OLED
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0,0);
  display.print("T:");
  display.print(temperatura,1);
  display.println(" C");

  display.print("Soil:");
  display.println(soilValue);

  display.print("MQ:");
  display.println(mqValue);

  display.print("LDR:");
  display.println(ldrValue);

  display.print("Lvl:");
  if(distancia < 0)
    display.println("--");
  else {
    display.print(distancia,1);
    display.println("cm");
  }

  // PRUEBA DEL VENTILADOR
  digitalWrite(FAN_PIN, HIGH); // encender
  digitalWrite(PUMP_PIN, HIGH); // encender  
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);
  digitalWrite(LED3_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);


  display.display();

  delay(1000);
}