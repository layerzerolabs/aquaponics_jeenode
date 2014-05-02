#include "Wire.h"

#include "pHSensor.h"
#define Write_Check      0x1234

//Our parameter, for ease of use and eeprom access lets use a struct
struct parameters_T
{
  unsigned int WriteCheck;
  int pH7Cal, pH4Cal;
  float pHStep;
} 
params;

const float vRef = 3.3; //Our vRef into the ADC wont be exact
                    //Since you can run VCC lower than Vref its
                    //best to measure and adjust here
const float opampGain = 5.25; //what is our Op-Amps gain (stage 1)


pHSensor::pHSensor(byte port, int address){
  this->address = address;
}

void pHSensor::setup() {  
  Wire.begin();
  params.WriteCheck = Write_Check;
  params.pH7Cal = 2048; //assume ideal probe and amp conditions 1/2 of 4096
  params.pH4Cal = 1286; //using ideal probe slope we end up this many 12bit units away on the 4 scale
  params.pHStep = 59.16;//ideal probe slope 
}

//Now that we know our probe "age" we can calucalate the proper pH Its really a matter of applying the math
//We will find our milivolts based on ADV vref and reading, then we use the 7 calibration
//to find out how many steps that is away from 7, then apply our calibrated slope to calcualte real pH
float pHSensor::calcpH(int raw)
{
 float miliVolts = (((float)raw/4096)*vRef)*1000;
 float temp = ((((vRef*(float)params.pH7Cal)/4096)*1000)- miliVolts)/opampGain;
 return 7-(temp/params.pHStep);
}

void pHSensor::measure(int * data) {
	//We'll assign 2 BYTES variables to capture the LSB and MSB(or Hi Low in this case)
	byte adc_high;
	byte adc_low;
	//We'll assemble the 2 in this variable
	int adc_result;
  
	Wire.requestFrom(address, 2);        //requests 2 bytes
	while(Wire.available() < 2);        //while two bytes to receive
	//Set em 
	adc_high = Wire.read();           
	adc_low = Wire.read();
	//now assemble them, remembering our byte maths a Union works well here as well
	adc_result = (adc_high * 256) + adc_low;
	data[0] = id;
	data[1] = 100*calcpH(adc_result);
	data[2] = 0;
}

char* pHSensor::getName() {
	return "pH";
}


