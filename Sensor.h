#include <JeeLib.h>

class Sensor {
    
  public:
    Sensor();
    virtual char* getName();
    virtual void setup();
    virtual int measure();
};
