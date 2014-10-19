#include "Sensor.h"
#include <Wire.h>

class pHSensor : public Sensor {
  
 public:
    static const int id = 5;
    char* getName();
    pHSensor(byte port, byte address, byte powerPortNo);
    void setup();
    void measure(int* data);
    
  private:
    int address;
    Port powerPort;
};
