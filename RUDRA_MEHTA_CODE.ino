#include <Wire.h>
#include <Adafruit_INA219.h>
#include <DHT.h>

// ===== OBJECTS =====
Adafruit_INA219 ina219;
DHT dht(4, DHT11);

// ===== PIN DEFINITIONS =====
#define LDR_PIN        35
#define RAIN_PIN       34

#define MOTOR_IN1      33       
#define MOTOR_IN2      27
#define MOTOR_ENA      25

#define PUMP_RELAY     14

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n===== SOLAR CLEANING SYSTEM FULL TEST =====");

  // I2C
  Wire.begin(21, 22);

  // INA219
  if (!ina219.begin()) {
    Serial.println("❌ INA219 not detected!");
    while (1);
  }
  Serial.println("✅ INA219 detected");

  // DHT
  dht.begin();
  Serial.println("✅ DHT11 started");

  // Pins
  pinMode(LDR_PIN, INPUT);
  pinMode(RAIN_PIN, INPUT);

  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_ENA, OUTPUT);

  pinMode(PUMP_RELAY, OUTPUT);

  // Initial states
  digitalWrite(PUMP_RELAY, LOW);
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_ENA, 0);

  Serial.println("✅ System ready\n");
}

void loop() {

  // ===== SOLAR PANEL (INA219) =====
  float busVoltage = ina219.getBusVoltage_V();
  float shuntVoltage = ina219.getShuntVoltage_mV();
  float current_mA = ina219.getCurrent_mA();
  float power_mW = ina219.getPower_mW();

  float panelVoltage = busVoltage + (shuntVoltage / 1000.0);

  // ===== SENSORS =====
  int ldrValue = analogRead(LDR_PIN);
  int rainValue = analogRead(RAIN_PIN);

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // ===== PRINT DATA =====
  Serial.println("------ SYSTEM DATA ------");

  Serial.print("Panel Voltage  : ");
  Serial.print(panelVoltage, 2);
  Serial.println(" V");

  Serial.print("Panel Current  : ");
  Serial.print(current_mA, 2);
  Serial.println(" mA");

  Serial.print("Panel Power    : ");
  Serial.print(power_mW / 1000.0, 3);
  Serial.println(" W");

  Serial.print("LDR Value      : ");
  Serial.println(ldrValue);

  Serial.print("Rain Sensor    : ");
  Serial.println(rainValue);

  Serial.print("Temperature   : ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity      : ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.println("-------------------------");

  // ===== MOTOR TEST =====
  Serial.println("Motor ON (Forward)");
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  analogWrite(MOTOR_ENA, 180);   // ~70% speed
  delay(3000);

  Serial.println("Motor OFF");
  analogWrite(MOTOR_ENA, 0);
  delay(2000);

  // ===== PUMP TEST =====
  Serial.println("Pump ON");
  digitalWrite(PUMP_RELAY, HIGH);
  delay(3000);

  Serial.println("Pump OFF");
  digitalWrite(PUMP_RELAY, LOW);

  Serial.println("\n===== CYCLE COMPLETE =====\n");
  delay(5000);
}
