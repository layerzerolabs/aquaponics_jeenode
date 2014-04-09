#include "BatterySensor.h"

BatterySensor::BatterySensor(byte port) {
  jeeport = new Port(port);
}

void BatterySensor::setup() {
  jeeport->mode(OUTPUT);
  jeeport->mode2(INPUT);
  jeeport->digiWrite(HIGH); // stops excess charge accumulating on cap
}

void BatterySensor::measure(int * readings) {
  readings[0] = 1; // sensor id
  jeeport->digiWrite(LOW);
  readings[1] = jeeport->anaRead();
  jeeport->digiWrite(HIGH);
  readings[2] = 0; // pad with 0 as no more data*/
}

char* BatterySensor::getName() {
  return "Battery";
}


