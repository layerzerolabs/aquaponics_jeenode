#include "Wire.h"
#include "pHSensor.h"

pHSensor::pHSensor(
  byte port, byte address, byte powerPortNo, int pH7Cal, int pH4Cal, float pHStep
) : powerPort(powerPortNo) {
  this->address = address;
  this->pH7Cal = pH7Cal;
  this->pH4Cal = pH4Cal;
  this->pHStep = pHStep;
}

void pHSensor::setup() {  
  powerPort.mode(OUTPUT);
  Wire.begin(); 
}

//Now that we know our probe "age" (i.e pHStep )we can calucalate the proper pH Its really a matter of applying the math
//We will find our milivolts based on ADV vref and reading, then we use the 7 calibration
//to find out how many steps that is away from 7, then apply our calibrated slope to calcualte real pH
float pHSensor::calcpH(int raw)
{
         Serial.print("pH7Cal: ");
       Serial.println(pH7Cal);
       Serial.print("pH4Cal: ");
       Serial.println(pH4Cal);
       Serial.print("pHStep: ");
       Serial.println(pHStep);
 float miliVolts = (((float)raw/4096)*vRef)*1000;
 float temp = ((((vRef*(float)pH7Cal)/4096)*1000)- miliVolts)/opampGain;
 return 7-(temp/pHStep);
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
	data[1] = 100*calcpH(adc_result);
	data[2] = adc_result; // raw value
}

char* pHSensor::getName() {
	return "pH";
}


