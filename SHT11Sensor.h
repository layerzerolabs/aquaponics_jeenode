#include "Sensor.h"
#include <PortsSHT11.h>

class SHT11Sensor : public Sensor {
     
  public:
    SHT11Sensor(byte);
    void setup();
    char* getName();
    void measure(int* readings); // output parameter
    
  private:
    SHT11* sht11;
};
