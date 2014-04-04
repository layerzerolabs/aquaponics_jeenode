#include <JeeLib.h>
#include "Sensor.cpp"
 

int additionalDelay = 1000; // between each reading process  

Sensor batterySensor;
Sensor battery2;

Sensor sensors[] = {batterySensor, battery2};

void setup() {
  for (int i = 0; i < sizeof(sensors); i ++) {
    sensors[i].setup(); 
  }
  setupRadio();
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < sizeof(sensors); i ++) {
    int reading = sensors[i].measure(); 
    Serial.println(reading);
    sendRadio(reading);
  }
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
