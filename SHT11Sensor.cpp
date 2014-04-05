#include "SHT11Sensor.h"
#include <avr/sleep.h>
#include <util/atomic.h>


SHT11Sensor::SHT11Sensor(byte port) {
  SHT11 sht11(port);
  this->sht11 = &sht11;
}

void SHT11Sensor::setup() {

}

int* SHT11Sensor::measure() {
  SHT11 rsht11 = *(sht11);
  rsht11.measure(SHT11::HUMI);
  rsht11.measure(SHT11::TEMP);  
  float h, t;
  rsht11.calculate(h, t);
  int humi = h + 0.5, temp = 10 * t + 0.5;
  int reading = humi;
  return reading;
}

char* SHT11Sensor::getName() {
  return "SHT11";
}


