// Configurator - a config getter and setter
/* Serial commands:

i     Display node id
i<nn> Set node id (resets system)
g     Display group
g<nn> Set group (resets system)
p<n>  Set the port number for subsequent commands to apply to
t     Display sensor type
t<n>  Set sensor type(resets system)
a     Display sensor I2C address
a<nn> Set I2C address of sensor (resets system)
4     Display pH4Cal 
4<n>  Enter reading for pH4 (resets system)
7     Display pH7Cal 
7<n>  Enter reading for pH7 (resets system)
s     Calculate and set pHStep
d     Delete config for sensor (resets system)
*/ 
#include <avr/eeprom.h>
#include <RF12.h>
#include <util/crc16.h>
/// For the EEPROM layout, see http://jeelabs.net/projects/jeelib/wiki/RF12demo
/// Useful url: http://blog.strobotics.com.au/2009/07/27/rfm12-tutorial-part-3a/

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

RF12Config mainConfig;

// Sensor configuration area
typedef struct {
    byte typeId;
    byte I2CAddress;
    byte powerPort;
    int pH7Cal;
    int pH4Cal;
    float pHStep;
} SensorConfig;

SensorConfig sensorConfigs[5];
word sensorCRC;

// EEPROM addresses
int sensorConfigsEepromAddr = (int)RF12_EEPROM_ADDR + sizeof mainConfig;
int sensorCRCEepromAddr = sensorConfigsEepromAddr + sizeof sensorConfigs;

void softReset(){
  Serial.println("Resetting ...");
  Serial.flush();
  delay(1000);
  asm volatile ("  jmp 0");
}

word calcCrc (const void* ptr, byte len) {
    word crc = ~0;
    for (byte i = 0; i < len; ++i)
        crc = _crc16_update(crc, ((const byte*) ptr)[i]);
    return crc;
}

void loadMainConfig () {
    eeprom_read_block(&mainConfig, RF12_EEPROM_ADDR, sizeof mainConfig);
}

void saveMainConfig () {
    mainConfig.crc = calcCrc(&mainConfig, sizeof mainConfig - 2);
    eeprom_write_block(&mainConfig, RF12_EEPROM_ADDR, sizeof mainConfig);
    softReset();
}
void loadsensorConfigs () {
    eeprom_read_block(&sensorConfigs, (void *)sensorConfigsEepromAddr, sizeof sensorConfigs);
    eeprom_read_block(&sensorCRC, (void *)sensorCRCEepromAddr, sizeof sensorCRC);
}

void savesensorConfigs () {
    sensorCRC = calcCrc(&sensorConfigs, sizeof sensorConfigs);
    eeprom_write_block(&sensorConfigs,  (void *)sensorConfigsEepromAddr, sizeof sensorConfigs);
    eeprom_write_block(&sensorCRC, (void *)sensorCRCEepromAddr, sizeof sensorCRC);
    softReset();
}

void writeDefaultMainConfig() {
      memset(&mainConfig, 0, sizeof mainConfig);
      mainConfig.nodeId = 0x81;       // 868 MHz, node 1
      mainConfig.group = 0xD3;        // default group 211
      mainConfig.frequency_offset = 1600;
      mainConfig.quiet_mode = true;   // Default flags, quiet on
      saveMainConfig();
}

float calcpHStep(byte port)
{
  //RefVoltage * our deltaRawpH / 12bit steps *mV in V / OP-Amp gain /pH step difference 7-4
  float opampGain = 5.25;
  float vRef = 3.3;
  return ((((vRef*(float)(sensorConfigs[port].pH7Cal - sensorConfigs[port].pH4Cal))/4096)*1000)/opampGain)/3;
}

void setup() {
  Serial.begin(9600);
  loadMainConfig();
  if (mainConfig.crc != calcCrc(&mainConfig, sizeof mainConfig - 2)) {
     Serial.print("Bad main config -- writing defaults"); 
     writeDefaultMainConfig();
     softReset();
  } else {
     Serial.println("Main config read as:");
    rf12_configDump(); 
  }
  loadsensorConfigs();
  if (sensorCRC != calcCrc(&sensorConfigs, sizeof sensorConfigs)) {
     Serial.println("Bad sensor config -- writing zeros"); 
      memset(&sensorConfigs, 0, sizeof sensorConfigs);
      savesensorConfigs();
  } else {
     Serial.println("Sensor config read as:"); 
     Serial.println();
     for (int port = 0; port <= 4; port ++) {
       Serial.print("Port: ");
       Serial.print(port);
       if (sensorConfigs[port].typeId != 0) {
       Serial.print("    Sensor type: ");
       Serial.println(sensorConfigs[port].typeId);
       if (sensorConfigs[port].I2CAddress != 0) {
         Serial.print("    which has I2C address of: ");
         Serial.println(sensorConfigs[port].I2CAddress);
       }
       if (sensorConfigs[port].powerPort != 0) {
         Serial.print("    powered by port: ");
         Serial.println(sensorConfigs[port].powerPort);
       }
       if (sensorConfigs[port].pH7Cal != 0) {
         Serial.print("    pH7Cal: ");
         Serial.println(sensorConfigs[port].pH7Cal);
       }
       if (sensorConfigs[port].pH4Cal != 0) {
         Serial.print("    pH4Cal: ");
         Serial.println(sensorConfigs[port].pH4Cal);
       }
       if (sensorConfigs[port].pHStep != 0) {
         Serial.print("    pHStep: ");
         Serial.println(sensorConfigs[port].pHStep);
       }
     }
     else Serial.println(" *** Not Used ***");
     }
     Serial.println();
  }
}
char command;
long param;
void loop() {
    if (Serial.available()) {  
        command = Serial.read();
        param = Serial.parseInt();
        runCommand(); 
    } 
}

byte sensorPort; // which sensor to apply sensor config to

void runCommand() {
  Serial.print("command: ");
  Serial.write(command);
  Serial.println();
  Serial.print("param: ");
  Serial.println(param);
  switch (command) {
    case 'i':
      if (!param) {
        Serial.print("My node id is: "); 
        Serial.println(mainConfig.nodeId  & 0x1F);
      } else if (param > 30) {
          Serial.println("New node id must be between 1 and 30 (inclusive)"); 
      } else {
        mainConfig.nodeId = (mainConfig.nodeId & 0xE0) + (param & 0x1F);
        Serial.print("Setting node id to: "); 
        Serial.println(mainConfig.nodeId & 0x1F);
        saveMainConfig();
      }
      break;
    case 'g':
      if (!param) {
        Serial.print("My group is: "); 
        Serial.println(mainConfig.group);
      } else if (param > 212) {
          Serial.println("New group must be between 1 and 212 (inclusive)"); 
      } else {
        mainConfig.group = param;
        Serial.print("Setting group to: "); 
        Serial.println(mainConfig.group);
        saveMainConfig();
      }
      break;
    case 'p':
      if (param > 4) {
         Serial.println("Port number must be between 1 and 4 (inclusive)");
      } else {
         sensorPort = param;
         Serial.print("Opening sensor for config on port ");
         Serial.println(sensorPort);
      }
      break;
     case 't':
       if (!param) {
           Serial.print("Sensor on port ");
           Serial.print(sensorPort);
           Serial.print(" is of type ");
           Serial.println(sensorConfigs[sensorPort].typeId);
       } else if (param > 5) {
           Serial.println("Sensor type must be between 1 and 5 (inclusive)");
       } else {
           sensorConfigs[sensorPort].typeId = param;
           Serial.print("Setting sensor on port ");
           Serial.print(sensorPort);
           Serial.print(" to type ");
           Serial.println(sensorConfigs[sensorPort].typeId);
           savesensorConfigs();
       }
       break;
       
     case 'a':
       if (!param) {
           Serial.print("Sensor on port ");
           Serial.print(sensorPort);
           Serial.print(" has the I2C address ");
           Serial.println(sensorConfigs[sensorPort].I2CAddress);
       } else if (param > 254) {
           Serial.println("Address must be decimal between 1 and 254 (inclusive)");
       } else {
           sensorConfigs[sensorPort].I2CAddress = param;
           Serial.print("Setting sensor on port ");
           Serial.print(sensorPort);
           Serial.print(" to I2C address ");
           Serial.println(sensorConfigs[sensorPort].I2CAddress);
           savesensorConfigs();
       }
       break;
              
     case 'v':
       if (!param) {
           Serial.print("Sensor on port ");
           Serial.print(sensorPort);
           Serial.print(" is powered by port ");
           Serial.println(sensorConfigs[sensorPort].powerPort);
       } else if (param < 1 || param > 4) {
           Serial.println("Power port must be between 1 and 4 (inclusive)");
       } else {
           sensorConfigs[sensorPort].powerPort = param;
           Serial.print("Setting sensor on port ");
           Serial.print(sensorPort);
           Serial.print(" to be powered by port ");
           Serial.println(sensorConfigs[sensorPort].powerPort);
           savesensorConfigs();
       }
       break;
              
     case '7':
       if (!param) {
           Serial.print("Sensor on port ");
           Serial.print(sensorPort);
           Serial.print(" has pH7Cal value ");
           Serial.println(sensorConfigs[sensorPort].pH7Cal);
       } else {
           sensorConfigs[sensorPort].pH7Cal = param;
           Serial.print("Calibrating sensor on port ");
           Serial.print(sensorPort);
           Serial.print(". Reading at pH 7 = ");
           Serial.println(sensorConfigs[sensorPort].pH7Cal);
           savesensorConfigs();
       }
              
     case '4':
       if (!param) {
           Serial.print("Sensor on port ");
           Serial.print(sensorPort);
           Serial.print(" has pH4Cal value ");
           Serial.println(sensorConfigs[sensorPort].pH4Cal);
       } else {
           sensorConfigs[sensorPort].pH4Cal = param;
           Serial.print("Calibrating sensor on port ");
           Serial.print(sensorPort);
           Serial.print(" Reading at pH 4 = ");
           Serial.println(sensorConfigs[sensorPort].pH4Cal);
           savesensorConfigs();
       }
       break;
              
     case 's':
       if (!param) {
           Serial.print("Calculating pH step value for sensor on port ");
           Serial.println(sensorPort);
           sensorConfigs[sensorPort].pHStep = calcpHStep(sensorPort);
           Serial.print("It is now set to: ");
           Serial.println(sensorConfigs[sensorPort].pHStep);
           savesensorConfigs();
       }
         
     case 'd':
         Serial.print("Deleting config for sensor on port ");
         Serial.println(sensorPort);
         memset(&sensorConfigs[sensorPort], 0, sizeof(SensorConfig));
         savesensorConfigs(); 
        break; 
  }
}
