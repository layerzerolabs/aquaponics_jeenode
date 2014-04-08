#include "Sensor.h"
#include <PortsSHT11.h>

class SHT11Sensor : public Sensor {
     
  public:
    char* getName();
    SHT11Sensor(byte port);
    void setup();
    void measure(int* readings); // output parameter
    
  private:
    byte port;
    SHT11* sht11;

};
