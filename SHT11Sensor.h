#include "Sensor.h"
#include <PortsSHT11.h>

class SHT11Sensor : public Sensor {
  private:
    byte port;
    SHT11* sht11;
    static const int settlePeriod = 50; // ms, waiting for voltage to settle 
  public:
    char* getName();
    SHT11Sensor(byte port);
    void setup();
    int measure();
};
