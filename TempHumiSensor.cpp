#include "TempHumiSensor.h"
#include <avr/sleep.h>
#include <util/atomic.h>


TempHumiSensor::TempHumiSensor(byte port): jeeport(port) {  
}

void TempHumiSensor::setup() {
}

void TempHumiSensor::measure(int * data) {
  jeeport.measure(SHT11::HUMI);
  jeeport.measure(SHT11::TEMP);  
  float h, t;
  jeeport.calculate(h, t);
  int humi = h + 0.5, temp = 10 * t + 0.5;
  data[0] = id; // sensor id
  data[1] = humi;
  data[2] = temp;
}

char* TempHumiSensor::getName() {
  return "TempHumi";
}


