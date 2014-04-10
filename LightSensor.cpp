#include "LightSensor.h"


LightSensor::LightSensor(byte port, int address) 
  // c++ initialise members in constructor
  : jeeport(port), luxplug(jeeport, (byte)address){}

void LightSensor::setup() {  
  luxplug.begin();
  luxplug.setGain(0); // Low gain
}

void LightSensor::measure(int * data) {
  luxplug.getData();
  data[0] = id;
  data[1] = luxplug.calcLux();
  data[2] = 0;
}

char* LightSensor::getName() {
  return "Light";
}


