
#include <JeeLib.h>
#include "Sensor.cpp"

int additionalDelay = 1000; // between each reading process  
Sensor batterySensor;

void setup() {
  batterySensor.setup();
  setupRadio();
  Serial.begin(9600);
}

void loop() {
  int reading = batterySensor.measure();
  Serial.println(reading);
  sendRadio(reading);
  delay(additionalDelay);
}

void setupRadio() {
  rf12_initialize(25, RF12_868MHZ, 101);
  rf12_easyInit(0);  
}

void sendRadio(int reading) {
  rf12_easyPoll();
  rf12_easySend(&reading, sizeof reading); 
}
