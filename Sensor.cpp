#include <JeeLib.h>

class Sensor {
  private:
    static const int voltagePin = A3;    // select the input pin
    static const int measurePin = 7;      // select the pin to measure on
    static const int settlePeriod = 50; // ms, waiting for voltage to settle 
    
  public:
    void setup() {
      pinMode(voltagePin, INPUT);
      pinMode(measurePin, OUTPUT);
      digitalWrite(measurePin, HIGH); // stops excess charge accumulating on cap
    }
    
    int measure() {
      digitalWrite(measurePin, LOW);
      delay(settlePeriod);
      int reading = analogRead(voltagePin);
      digitalWrite(measurePin, HIGH);
      return reading;
    }
};
