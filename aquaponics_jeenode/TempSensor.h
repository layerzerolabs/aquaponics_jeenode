#include "Sensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

class TempSensor : public Sensor {
     
  public:
    static const int id = 4;
    TempSensor(byte port);
    void setup();
    char* getName();
    void measure(int* data); // output parameter
    
  private:
    Port jeeport;
    OneWire oneWire;
    DallasTemperature dallas;
    DeviceAddress deviceAddress;
    static const int resolution = 12;    // 9 bit resolution by default
    static const int delay = 750/ (1 << (12-resolution));
};
