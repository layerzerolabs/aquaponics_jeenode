#include "BatterySensor.h"

BatterySensor::BatterySensor(int voltagePin, int controlPin) {
  this->voltagePin = voltagePin;
  this->controlPin = controlPin;
}

void BatterySensor::setup() {
  pinMode(this->voltagePin, INPUT);
  pinMode(this->controlPin, OUTPUT);
  digitalWrite(this->controlPin, HIGH); // stops excess charge accumulating on cap
}

void BatterySensor::measure(int * readings) {
  digitalWrite(this->controlPin, LOW);
  delay(50); // settle period
  readings[0] = analogRead(this->voltagePin);
  digitalWrite(this->controlPin, HIGH);
  readings[1] = 0; // pad
}

char* BatterySensor::getName() {
  return "Battery";
}

