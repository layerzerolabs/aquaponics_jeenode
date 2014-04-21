#include "LightSensor.h"


LightSensor::LightSensor(byte port, int address) 
  // c++ initialise members in constructor
  : jeeport(port), luxplug(jeeport, (byte)address){}

void LightSensor::setup() {  

}

void LightSensor::measure(int * data) {
  luxplug.begin();
  luxplug.setGain(0); // Low gain 
  Sleepy::loseSomeTime(700); // wait for power up
  luxplug.getData();
  data[0] = id;
  data[1] = luxplug.calcLux();
  data[2] = 0;
  luxplug.poweroff(); // Power off when we've got the data.
}

char* LightSensor::getName() {
  return "Light";
}


