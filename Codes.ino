#include <SoftwareSerial.h>
#include <DHT.h>

// Flame Sensor
const int flamePin = A0;
const int flameLED = 13;

// Smoke Sensor
const int smokePinAnalog = A1;

// Temperature Sensor
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Motion Sensor
int sensorPin = 4; // PIR sensor OUT pin connected to digital pin 4
const int motionLED = 5;

// Bluetooth Module
// Define the pins for the SoftwareSerial communication
#define BT_TX_PIN 6  // TX pin of Arduino connected to RX pin of HC-06
#define BT_RX_PIN 7  // RX pin of Arduino connected to TX pin of HC-06
SoftwareSerial BTSerial(BT_RX_PIN, BT_TX_PIN); // Create a SoftwareSerial object

void setup() {
  // Flame Sensor
  pinMode(flamePin, INPUT);
  pinMode(flameLED, OUTPUT);
  
  // Smoke Sensor
  pinMode(smokePinAnalog, INPUT);

  // Temperature Sensor
  dht.begin();

  // Motion Sensor
  pinMode(sensorPin, INPUT);
  pinMode(motionLED, OUTPUT);

  // Bluetooth Module
  BTSerial.begin(9600);
  
  Serial.begin(9600);
}

void loop() {
    // Temperature Sensor
  delay(2000);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (!isnan(temperature)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }
  
  // Flame Sensor
  int flameSensorValue = analogRead(flamePin);
  if (flameSensorValue > 1000000) {
    Serial.println("Flame detected!");
    digitalWrite(flameLED, HIGH);
  } else {
    Serial.println("No flame detected");
    digitalWrite(flameLED, LOW);
  }

  // Smoke Sensor
  int smokeSensorValueAnalog = analogRead(smokePinAnalog);
    if (smokeSensorValueAnalog > 1000) {
    Serial.println("Fire detected!");
  } else {
    Serial.println("No smoke detected. Place is safe");
  }

  // Motion Sensor
  int motionSensorState = digitalRead(sensorPin);
  if (motionSensorState == HIGH) {
    digitalWrite(motionLED, HIGH);
    Serial.println("Motion detected!");
  } else {
    digitalWrite(motionLED, LOW);
    Serial.println("No motion detected");
  }

  // Bluetooth Module
  // Send sensor data over Bluetooth
  BTSerial.print("Flame: ");
  BTSerial.println(flameSensorValue);
  BTSerial.print("Smoke: ");
  BTSerial.println(smokeSensorValueAnalog);
  BTSerial.print("Temperature: ");
  BTSerial.print(temperature);
  BTSerial.println(" °C");
  BTSerial.print("Humidity: ");
  BTSerial.print(humidity);
  BTSerial.println("%");
  BTSerial.println("----------------------");
  delay(1000); // Wait for a second before sending the next set of sensor data
}
