#include "TempSensor.h"
#include <avr/sleep.h>
#include <util/atomic.h>


TempSensor::TempSensor(byte port): jeeport(port),
  // arduino pin = port no + 3 http://jeelabs.org/2013/03/22/jeenode-v6-reference/
  oneWire(port + 3), dallas(&oneWire) 
  {  
}

void TempSensor::setup() {
  dallas.getAddress(deviceAddress, 0);
  dallas.setResolution(deviceAddress, resolution);
  dallas.setWaitForConversion(false);  // makes it async
}

void TempSensor::measure(int * data) {
  
  dallas.requestTemperatures();
  
  Sleepy::loseSomeTime(delay);
  
  data[0] = id;
  data[1] = dallas.getTempCByIndex(0)*10;
  data[2] = 0;
}

char* TempSensor::getName() {
  return "Temp";
}


