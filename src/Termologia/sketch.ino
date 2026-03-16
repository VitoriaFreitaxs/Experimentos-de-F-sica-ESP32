#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definições de pinos
#define ONE_WIRE_BUS 4  // GPIO4 do ESP32 conectado ao DATA do DS18B20

// Inicialização do OneWire e DallasTemperature
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Inicialização do LCD - Endereço padrão 0x27 (às vezes 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2); // 16 colunas, 2 linhas

void setup() {
  // Inicializa comunicação serial
  Serial.begin(115200);

  // Inicializa sensor de temperatura
  sensors.begin();

  // Inicializa LCD
  lcd.init();
  lcd.backlight();

  // Mensagem inicial no LCD
  lcd.setCursor(0, 0);
  lcd.print("Sensor Temp:");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Solicita leitura dos sensores
  sensors.requestTemperatures();
  
  // Lê a temperatura em Celsius
  float temperatureC = sensors.getTempCByIndex(0);

  // Exibe no Serial Monitor
  Serial.print("Temperatura: ");
  Serial.print(temperatureC);
  Serial.println(" C");

  // Exibe no LCD
  lcd.clear();
  lcd.setCursor(6, 6);
  lcd.print("Temp:");
  lcd.setCursor(6, 0);
  lcd.print(temperatureC);
  lcd.print((char)223); // Símbolo de grau
  lcd.print("C");

  // Aguarda um tempo para próxima leitura
  delay(1000);
}
