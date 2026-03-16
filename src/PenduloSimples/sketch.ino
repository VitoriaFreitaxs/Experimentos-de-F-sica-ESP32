#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Sensor KY-032
const int sensorPin = 18;

// Constantes físicas do pêndulo
const float L = 0.30; // comprimento em metros
const float g = 9.8;  // gravidade
const float periodoTeorico = 2 * PI * sqrt(L / g);

// Controle de tempo
unsigned long tempoAnterior = 0;
unsigned long tempoAtual = 0;
unsigned long tempoUltimoMovimento = 0;

bool esperandoRetorno = false;
const int MAX_CICLOS = 20;
float periodos[MAX_CICLOS];
int ciclos = 0;

const unsigned long tempoMaximoParado = 3000000; // 3 segundos em micros

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);

  Wire.begin(21, 22);  // SDA, SCL para ESP32
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Pendulo Iniciado");
  delay(2000);
  lcd.clear();
}

void loop() {
  int leitura = digitalRead(sensorPin);
  unsigned long agora = micros();

  // Sensor detecta obstáculo (passagem da massa)
  if (leitura == LOW && !esperandoRetorno) {
    tempoAtual = agora;

    if (tempoAnterior > 0 && ciclos < MAX_CICLOS) {
      unsigned long duracaoMicros = tempoAtual - tempoAnterior;
      float periodo = duracaoMicros / 1000000.0;
      periodos[ciclos] = periodo;

      Serial.print("Ciclo ");
      Serial.print(ciclos + 1);
      Serial.print(": ");
      Serial.print(periodo, 3);
      Serial.println(" s");

      ciclos++;
    }

    tempoAnterior = tempoAtual;
    tempoUltimoMovimento = agora;
    esperandoRetorno = true;
    delay(10); // debounce
  }

  // Aguarda o pêndulo sair do campo do sensor
  if (leitura == HIGH && esperandoRetorno) {
    esperandoRetorno = false;
  }

  // Verifica se o pêndulo pa

