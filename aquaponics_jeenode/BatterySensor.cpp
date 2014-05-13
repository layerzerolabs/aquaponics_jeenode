#include "BatterySensor.h"

BatterySensor::BatterySensor(byte port) : jeeport(port) {}

void BatterySensor::setup() {
  jeeport.digiWrite(HIGH); // stops current leaking across
  jeeport.mode(OUTPUT);
  jeeport.mode2(INPUT);
}

void BatterySensor::measure(int * data) {
  data[0] = id;
  jeeport.digiWrite(LOW);
  data[1] = jeeport.anaRead();
  jeeport.digiWrite(HIGH); // stops current leaking across
  data[2] = 0; // pad with 0 as no more data
}

char* BatterySensor::getName() {
  return "Battery";
}


