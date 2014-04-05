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

int* BatterySensor::measure() {
  digitalWrite(this->controlPin, LOW);
  delay(50); // settle period
  int reading = analogRead(this->voltagePin);
  digitalWrite(this->controlPin, HIGH);
  // if not static, falls from scope.
  static int readings[2] = {reading};
  return readings;
}

char* BatterySensor::getName() {
  return "Battery";
}

