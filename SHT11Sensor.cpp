#include "SHT11Sensor.h"

SHT11Sensor::SHT11Sensor(int voltagePin, int controlPin) {
  this->voltagePin = voltagePin;
  this->controlPin = controlPin;
}

void SHT11Sensor::setup() {
  pinMode(this->voltagePin, INPUT);
  pinMode(this->controlPin, OUTPUT);
  digitalWrite(this->controlPin, HIGH); // stops excess charge accumulating on cap
}

int SHT11Sensor::measure() {
  digitalWrite(this->controlPin, LOW);
  delay(50); // settle period
  int reading = analogRead(this->voltagePin);
  digitalWrite(this->controlPin, HIGH);
  return reading;
}

char* SHT11Sensor::getName() {
  return "SHT11";
}

