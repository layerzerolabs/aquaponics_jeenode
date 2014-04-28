#include <JeeLib.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "BatterySensor.h"
#include "TempHumiSensor.h" 
#include "LightSensor.h"
#include "TempSensor.h"
#include <avr/eeprom.h>
#include <RF12.h>

// RF12 configuration area
typedef struct {
    byte nodeId          ; // used by rf12_config, offset 0
    byte group           ;  // used by rf12_config, offset 1
    byte format          ;    // used by rf12_config, offset 2
    byte hex_output      :2;    // 0 = dec, 1 = hex, 2 = hex+ascii
    byte collect_mode    :1;    // 0 = ack, 1 = don't send acks
    byte quiet_mode      :1;    // 0 = show all, 1 = show only valid packets
    byte spare_flags     :4;
    word frequency_offset; // used by rf12_config, offset 4
    byte pad[RF12_EEPROM_SIZE-8];
    word crc;
} RF12Config;

// Sensor configuration area
typedef struct {
    byte typeId;
    byte I2CAddress;
} SensorConfig;

SensorConfig sensorConfig[4];

int sensorConfigEepromAddr = (int)RF12_EEPROM_ADDR + sizeof(RF12Config);


int waitMillis = 1000; // between each reading process 

Sensor* sensors[5];
int payload[3]; // id and up to two readings

ISR(WDT_vect) { Sleepy::watchdogEvent(); } // needed for power-down

// C way for getting size of array
int numSensors = sizeof(sensors) / sizeof(sensors[0]);

void setup() {
  Serial.begin(9600);
  eeprom_read_block(&sensorConfig, (void *)sensorConfigEepromAddr, sizeof sensorConfig);

  // load the sensor for each port
  for (int port = 0; port < 5; port ++) {
   switch (sensorConfig[port].typeId) {
      case 1: 
       sensors[port] = new BatterySensor(port);
       break;
      case 2:
       sensors[port] = new TempHumiSensor(port);
       break;
      case 3:
       sensors[port] = new LightSensor(port, sensorConfig[port].I2CAddress);
       break;
      case 4:
       sensors[port] = new TempSensor(port);
       break;
   }
  }
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
  rf12_config();
}

void sendRadio() {
  rf12_sleep(RF12_WAKEUP);
  rf12_sendNow(0, &payload, sizeof payload); 
  Serial.flush();
  rf12_sendWait(2);
  rf12_sleep(RF12_SLEEP);
}
