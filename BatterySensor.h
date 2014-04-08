#include "Sensor.h"

class BatterySensor : public Sensor {
  
 public:
    char* getName();
    BatterySensor(byte port);
    void setup();
    void measure(int* readings);
    
  private:
    Port * jeeport;
};
