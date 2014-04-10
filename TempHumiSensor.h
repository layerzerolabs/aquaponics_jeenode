#include "Sensor.h"
#include <PortsSHT11.h>

class TempHumiSensor : public Sensor {
     
  public:
    static const int id = 2;
    TempHumiSensor(byte);
    void setup();
    char* getName();
    void measure(int* data); // output parameter
    
  private:
    SHT11 jeeport;
};
