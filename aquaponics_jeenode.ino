#include <JeeLib.h>
#include "BatterySensor.h"
#include "SHT11Sensor.h" 

int voltagePin = A1; // for battery
int controlPin = 5;  // for battery
byte port = 1;       // for SHT11

/*****************************************************
  EDIT FOR DIFFERENT COMBINATIONS OF SENSORS
*****************************************************/
  
BatterySensor batterySensor(voltagePin, controlPin);
SHT11Sensor sht11Sensor(port);

Sensor* sensors[] = {
  &batterySensor,
  &sht11Sensor
};

/*****************************************************/

int nodeId = 25;

int payload[3]; // id and up to two readings

int additionalDelay = 1000; // between each reading process 
ISR(WDT_vect) { Sleepy::watchdogEvent(); } // needed for power-down

// C way for getting size of array
int numSensors = sizeof(sensors) / sizeof(sensors[0]);

void setup() {
  for (int i = 0; i < numSensors; i ++) {
    sensors[i]->setup(); 
  }
  setupRadio();
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < numSensors; i ++) {
    sensors[i]->measure(payload);
    Serial.print("Sensor:");
    Serial.print(sensors[i]->getName());
    Serial.print(" ID:");
    Serial.print(payload[0]);
    Serial.print(" DATA0:");
    Serial.print(payload[1]);
    Serial.print(" DATA1:");
    Serial.println(payload[2]);
    sendRadio();
  }
  Serial.flush();
  Sleepy::loseSomeTime(additionalDelay);
}

void setupRadio() {
  rf12_initialize(nodeId, RF12_868MHZ, 101); // 101 = group id
  rf12_easyInit(0);  
}

void sendRadio() {
  rf12_easyPoll();
  rf12_easySend(&payload, sizeof payload); 
}
