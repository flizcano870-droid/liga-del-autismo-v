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

  const int TRIG_PIN = 2;
  const int ECHO_PIN = 3;
  const int LED1_PIN = 4;
  const int LED2_PIN = 5;
  const int LED3_PIN = 6;

  const int TEMP_PIN = 7;
  const int PUMP_PIN = 8;
  const int FAN1_PIN = 9;
  const int FAN2_PIN = 10;
  const int BUZZER_PIN = 11;

  //Alertas y umbrales
  const int MQ_ALERTA = 150;
  const int MQ_PELIGRO = 400;

  // Ajusta este valor según tus pruebas con el LDR
  const int LDR_DIA = 500;

  // Temperaturas (°C)
  const float TEMP_MAX_DIA = 18.0;
  const float TEMP_MIN_DIA = 14.0;

  const float TEMP_MAX_NOCHE = 8.0;
  const float TEMP_MIN_NOCHE = 5.0;

  bool bombaProbada = false;

  // ===== Promedio de temperatura =====
  const int NUM_MUESTRAS = 10;
  float temperaturas[NUM_MUESTRAS];
  int indiceTemp = 0;
  bool bufferLleno = false;

  // ===== Histéresis =====
  const float HISTERESIS = 1.0;

  // Estado de actuadores
  bool ventiladoresEncendidos = false;
  bool calefactorEncendido = false;

  // ===== Buzzer no bloqueante =====
  unsigned long tiempoAnteriorBuzzer = 0;
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

  void setup() {

    Serial.begin(9600);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(LED3_PIN, OUTPUT);

    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(FAN1_PIN, OUTPUT);
    pinMode(FAN2_PIN, OUTPUT);
    pinMode(TEMP_PIN, OUTPUT);
    pinMode(PUMP_PIN, OUTPUT);


    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(FAN1_PIN, LOW);
    digitalWrite(FAN2_PIN, LOW);
    digitalWrite(PUMP_PIN, LOW);
    

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println("Error OLED");
      while (1);
    }

    display.clearDisplay();
    display.display();
    
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
    float temperatura = (voltaje * 100.0)-10 ;

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

    // Ultrasonico
    float distancia = medirDistancia();

    // Monitor serial
    Serial.print("Temp: ");
    Serial.print(temperaturaProm);
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
    display.print(temperaturaProm,1);
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

  // Pulso de la bomba (solo una vez)
  if (!bombaProbada) {

    digitalWrite(PUMP_PIN, HIGH);
    delay(1500);  // 1.5 segundos

    digitalWrite(PUMP_PIN, LOW);

    bombaProbada = true;
  }

  display.display();

  delay(100);
  }