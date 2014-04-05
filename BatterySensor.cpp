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

int BatterySensor::measure() {
  digitalWrite(this->controlPin, LOW);
  delay(50); // settle period
  int reading = analogRead(this->voltagePin);
  digitalWrite(this->controlPin, HIGH);S
  return reading;
}

char* BatterySensor::getName() {
  return "Battery";
}

