#include <JeeLib.h>

#ifndef SENSOR_H
#define SENSOR_H
class Sensor {
    
  public:
    Sensor();
    virtual char* getName();
    virtual void setup();
    virtual int* measure();
};
#endif
