#include "Sensor.h"

class BatterySensor : public Sensor {
  
 public:
    static const int id = 1;
    char* getName();
    BatterySensor(byte port);
    void setup();
    void measure(int* readings);
    
  private:
    Port * jeeport;
};
