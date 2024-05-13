// #include <SoftwareSerial.h>
#include <DHT.h>
bool flame_state = 0, smoke_state = 0, motion_state = 0;

// Flame Sensor
const int flamePin = A0;
const int flameLED = 13;
String flame_reading;

// Smoke Sensor
const int smokePinAnalog = A1;
String smoke_reading;

// Temperature Sensor1 
#define DHTPIN1 2
#define DHTTYPE1 DHT11
DHT dht1(DHTPIN1, DHTTYPE1);

// Temperature Sensor2 
#define DHTPIN2 6
#define DHTTYPE2 DHT11
DHT dht2(DHTPIN2, DHTTYPE2);

// Motion Sensor
const int sensorPin = 4;  // PIR sensor OUT pin connected to digital pin 4
String motion_reading;

// Fan
const int fanPin = 8;  // Replace with the actual pin connected to your fan
int fanSpeed = 200;  // Adjust this value to change the speed (0-255)

// Water pump
const int pumpPin = 9;  // Change this to match your setup

// LED
const int motionLED = 5;

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);

  // Flame Sensor
  pinMode(flamePin, INPUT);
  pinMode(flameLED, OUTPUT);

  // Smoke Sensor
  pinMode(smokePinAnalog, INPUT);

  // Temperature Sensor
  dht1.begin();
  dht2.begin();
  // Motion Sensor
  pinMode(sensorPin, INPUT);
  pinMode(motionLED, OUTPUT);

  // Fan
  pinMode(fanPin, OUTPUT);  // Set the fan pin as output

  // Water pump
  pinMode(pumpPin, OUTPUT);  // Set the relay pin as an output
}

void loop() {
  // Flame Sensor
  int flameSensorValue = analogRead(flamePin);
  if (flameSensorValue <30) {
    if (flame_state == 0) {
      flame_state = 1;
      flame_reading = "Fire";
      digitalWrite(pumpPin, LOW);
    }
  } else if (flame_state == 1) {
    flame_state = 0;
    flame_reading = "No fire";
    digitalWrite(pumpPin, HIGH);
  } else {
    flame_reading = "No fire";
    
   
  }

  // Smoke Sensor
  int smokeSensorValueAnalog = analogRead(smokePinAnalog);
  if (smokeSensorValueAnalog >176) {  // Adjust threshold as per your smoke sensor's sensitivity
    if (smoke_state == 0) {
      smoke_state = 1;
      smoke_reading = "Smoke";
      digitalWrite(fanPin, LOW);  // Turn on the fan if smoke is detected
    }
  } else if (smoke_state == 1) {
    smoke_state = 0;
    digitalWrite(fanPin, HIGH);  // Turn off the fan if no smoke is detected
    smoke_reading = "No smoke";
  } else {
    smoke_reading = "No smoke";
  }

  // Motion Sensor
  int motionSensorState = digitalRead(sensorPin);
  if (motionSensorState == HIGH) {
    digitalWrite(motionLED, HIGH);
    motion_reading = "Motion";
  } else {
    digitalWrite(motionLED, LOW);
    motion_reading = "No move";
  }
  // Temperature Sensor
  delay(2000);

  float temperaturein = dht1.readTemperature();
  if (!isnan(temperaturein)) {
    Serial.print(temperaturein);
    Serial.print("\n");
  } else {
    Serial.print(0);
    Serial.print("\n");
  } 
  float temperatureout = dht2.readTemperature();
  if (!isnan(temperatureout)) {
    Serial.print(temperatureout);
    Serial.print("\n");
  } else {
    Serial.print(0);
    Serial.print("\n"); } 


  Serial.print(flame_reading);
  Serial.print("\n");

  Serial.print(smoke_reading);
  Serial.print("\n");

  Serial.print(motion_reading);
  Serial.print("\n");

  delay(1000);  // Wait for a second before sending the next set of sensor data
}