#include <JeeLib.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "BatterySensor.h"
#include "TempHumiSensor.h" 
#include "LightSensor.h"
#include "TempSensor.h"

/*****************************************************
  EDIT FOR DIFFERENT COMBINATIONS OF SENSORS
*****************************************************/

int nodeId = 23;

//TempHumiSensor tempHumiSensor(1); // jeeport 1
BatterySensor batterySensor(3);
LightSensor lightSensor(4, 0x39); // jeeport and I2C address
TempSensor tempSensor(2);

Sensor* sensors[] = {
  //&tempHumiSensor,
  &batterySensor,
  &lightSensor,
  &tempSensor
};

int waitMillis = 1000; // between each reading process 

/*****************************************************/

int payload[3]; // id and up to two readings

ISR(WDT_vect) { Sleepy::watchdogEvent(); } // needed for power-down

// C way for getting size of array
int numSensors = sizeof(sensors) / sizeof(sensors[0]);

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < numSensors; i ++) {
    sensors[i]->setup(); 
  }
  setupRadio();
}

void loop() {
  for (int i = 0; i < numSensors; i ++) {
    sensors[i]->measure(payload);
    Serial.print("Sensor:");
    Serial.print(sensors[i]->getName());
    Serial.print(" ID:");
    Serial.print(payload[0]);
    Serial.print(" READING0:");
    Serial.print(payload[1]);
    Serial.print(" READING1:");
    Serial.println(payload[2]);
    sendRadio();
  }
  Serial.flush();
  Sleepy::loseSomeTime(waitMillis);
}

void setupRadio() {
  rf12_initialize(nodeId, RF12_868MHZ, 101); // 101 = group id
}

void sendRadio() {
  rf12_sleep(RF12_WAKEUP);
  rf12_sendNow(0, &payload, sizeof payload); 
  Serial.flush();
  rf12_sendWait(2);
  rf12_sleep(RF12_SLEEP);
}
