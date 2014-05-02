#include "Sensor.h"
#include <Wire.h>

class pHSensor : public Sensor {
  
 public:
    static const int id = 5;
    char* getName();
    pHSensor(byte port, int address);
    void setup();
    void measure(int* data);
    
  private:
    float calcpH(int raw);
    int address;
};
