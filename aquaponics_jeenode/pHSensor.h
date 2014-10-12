#include "Sensor.h"
#include <Wire.h>

class pHSensor : public Sensor {
  
 public:
    static const int id = 5;
    static const float vRef = 3.3;
    static const float opampGain = 5.25;
    char* getName();
    pHSensor(byte port, byte address, byte powerPortNo);
    void setup();
    void measure(int* data);
    
  private:
    float calcpH(int raw);
    int address;
    Port powerPort;
};
