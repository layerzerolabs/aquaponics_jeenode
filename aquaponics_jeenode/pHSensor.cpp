#include "Wire.h"
#include "pHSensor.h"

pHSensor::pHSensor(byte port, byte address, byte powerPortNo) : powerPort(powerPortNo) {
  this->address = address;
}

void pHSensor::setup() {  
  powerPort.mode(OUTPUT);
  Wire.begin(); 
}


void pHSensor::measure(int * data) {
	//We'll assign 2 BYTES variables to capture the LSB and MSB(or Hi Low in this case)
	byte adc_high;
	byte adc_low;
	//We'll assemble the 2 in this variable
	int adc_result;

        // Turn on the sensor and wait for it to settle
        powerPort.digiWrite(HIGH);
        Sleepy::loseSomeTime(5000);
        // Read the sensor
	Wire.requestFrom(address, 2);        //requests 2 bytes
	while(Wire.available() < 2); 
	adc_high = Wire.read();           
	adc_low = Wire.read();
        powerPort.digiWrite(LOW); // turn it off
    
	//now assemble them, remembering our byte maths a Union works well here as well
	adc_result = (adc_high * 256) + adc_low;
	data[0] = id;
	data[1] = adc_result; // raw value
}

char* pHSensor::getName() {
	return "pH";
}


