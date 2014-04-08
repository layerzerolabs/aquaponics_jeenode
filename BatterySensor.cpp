#include "BatterySensor.h"

BatterySensor::BatterySensor(byte port) {
  //jeeport = new Port(port);
}

void BatterySensor::setup() {
  Port jport(2);
  jport.mode(INPUT);
  jport.mode2(OUTPUT);
  jport.digiWrite(1); // stops excess charge accumulating on cap
  Serial.println(jport.digiRead());
  jport.digiWrite(LOW); 
  Serial.println(jport.digiRead());
}

void BatterySensor::measure(int * readings) {
  /*jport.digiWrite(LOW);
  readings[0] = 1; // sensor id
  readings[1] = analogRead(A1);
  jport.digiWrite(HIGH);
  readings[2] = 0; // pad with 0 as no more data*/
}

char* BatterySensor::getName() {
  return "Battery";
}


