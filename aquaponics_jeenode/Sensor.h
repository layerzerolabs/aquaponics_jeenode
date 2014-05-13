#include <JeeLib.h>

#ifndef SENSOR_H
#define SENSOR_H
class Sensor {
    
  public:
    Sensor(byte port = 0);
    virtual char* getName();
    virtual void setup();
    virtual void measure(int* data);
};
#endif
