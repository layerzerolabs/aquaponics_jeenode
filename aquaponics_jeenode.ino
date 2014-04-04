#include <JeeLib.h>
#include "BatterySensor.h"
 
int nodeId = 25;

int additionalDelay = 1000; // between each reading process  

// battery sensor
int voltagePin = A1;
int controlPin = 5;
BatterySensor batterySensor(voltagePin, controlPin);
BatterySensor batterySensor2(voltagePin, controlPin);

Sensor* sensors[] = {&batterySensor, &batterySensor2};

int numSensors = sizeof(sensors) / sizeof(sensors[0]);

void setup() {
  batterySensor.setup();
  for (int i = 0; i < numSensors; i ++) {
    sensors[i]->setup(); 
  }
  setupRadio();
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < numSensors; i ++) {
    int reading = sensors[i]->measure(); 
    Serial.print(sensors[i]->getName());
    Serial.print(": ");
    Serial.println(reading);
    sendRadio(reading);
  }
  delay(additionalDelay);
}

void setupRadio() {
  rf12_initialize(nodeId, RF12_868MHZ, 101);
  rf12_easyInit(0);  
}

void sendRadio(int reading) {
  rf12_easyPoll();
  rf12_easySend(&reading, sizeof reading); 
}
