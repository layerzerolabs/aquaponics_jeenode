#include "Sensor.h"

class LightSensor : public Sensor {
  
 public:
    static const int id = 3;
    char* getName();
    LightSensor(byte port, int address);
    void setup();
    void measure(int* data);
    
  private:
    PortI2C jeeport;
    LuxPlug luxplug;
};
