#include "Sensor.h"

class SHT11Sensor : public Sensor {
  private:
    int voltagePin;
    int controlPin;
    static const int settlePeriod = 50; // ms, waiting for voltage to settle 
   
  public:
    char* getName();
    RoomNode(int voltagePin, int controlPin);
    void setup();
    int measure();
};
