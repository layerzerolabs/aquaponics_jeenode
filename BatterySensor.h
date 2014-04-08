#include "Sensor.h"

class BatterySensor : public Sensor {
  
 public:
    char* getName();
    BatterySensor(int voltagePin, int controlPin);
    void setup();
    void measure(int* readings);
    
  private:
    int voltagePin;
    int controlPin;
};
