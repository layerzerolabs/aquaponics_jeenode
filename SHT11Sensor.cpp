#include "SHT11Sensor.h"
#include <avr/sleep.h>
#include <util/atomic.h>


SHT11Sensor::SHT11Sensor(byte port) {
  sht11 = new SHT11(port);
}

void SHT11Sensor::setup() {

}

int* SHT11Sensor::measure() {
  sht11->measure(SHT11::HUMI);
  sht11->measure(SHT11::TEMP);  
  float h, t;
  sht11->calculate(h, t);
  int humi = h + 0.5, temp = 10 * t + 0.5;
  int readings[2] = {humi, temp};
    Serial.println(readings[0]);
  return readings;
}

char* SHT11Sensor::getName() {
  return "SHT11";
}


