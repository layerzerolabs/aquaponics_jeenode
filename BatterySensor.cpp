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
  readings[0] = 1; // sensor id
  readings[1] = analogRead(this->voltagePin);
  digitalWrite(this->controlPin, HIGH);
  readings[2] = 0; // pad with 0 as no more data
}

char* BatterySensor::getName() {
  return "Battery";
}


