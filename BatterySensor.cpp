#include "BatterySensor.h"

BatterySensor::BatterySensor(byte port) {
  jeeport = new Port(port);
}

void BatterySensor::setup() {
  jeeport->mode(OUTPUT);
  jeeport->mode2(INPUT);
  jeeport->digiWrite(HIGH); // stops excess charge accumulating on cap
}

void BatterySensor::measure(int * data) {
  data[0] = id;
  jeeport->digiWrite(LOW);
  data[1] = jeeport->anaRead();
  jeeport->digiWrite(HIGH);
  data[2] = 0; // pad with 0 as no more data
}

char* BatterySensor::getName() {
  return "Battery";
}


