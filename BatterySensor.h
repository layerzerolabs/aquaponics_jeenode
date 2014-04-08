#include "Sensor.h"

class BatterySensor : public Sensor {
  private:
    int voltagePin;
    int controlPin;
    static const int settlePeriod = 50; // ms, waiting for voltage to settle 
   
  public:
    char* getName();
    BatterySensor(int voltagePin, int controlPin);
    void setup();
    void measure(int*);
};
