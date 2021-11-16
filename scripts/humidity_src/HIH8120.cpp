/*
    Filename - HIH8120.cpp
    Author - Aron Kjærgaard, Kiril Iliev, Martin Tsvetkov
    Date - 26/10/2021
*/

#include "HIH8120.h"
#include <iostream>
#include <unistd.h>


using namespace std;

namespace I2C_HIH8120 {

HIH8120::HIH8120(unsigned int I2CBus, unsigned int I2CAddress)
		:I2CDevice(I2CBus, I2CAddress){
	this->I2CAddress = I2CAddress;
	this->I2CBus = I2CBus;
	this->temperature = 0;
	this->humidity = 0;
}

/*
    Name - getMeasurement
    Purpose - To fetch the humidity and temperature from the HIH8120 sensor
    Description of inputs - There are none
    Description of return values - Returns 0 if successful and sets the humidity and temperature
*/
int HIH8120::getMeasurement(){
	this->prepareMeasurement();
	unsigned char * deviceData = this->readDevice(4);
	if (deviceData[0] >= 0b01000000) {
		return 1;
	}
	
	this->humidity = this->combineRegisters(deviceData[0], deviceData[1]);
	this->temperature = this->combineRegisters(deviceData[2], deviceData[3]) >> 2;
	return 0;
}

//Fetches the humidity
float HIH8120::getHumidity(){
	getMeasurement();
	return (this->humidity * 100) / 16382.0f;
}

//Fetches the temperature
float HIH8120::getTemperature(){
	getMeasurement();
	return ((this->temperature * 165) / 16382.0f) -40;
}

//Wakes up the HIH8120 sensor by sending it package and then waiting
void HIH8120::prepareMeasurement(){
	this->write((char)0);
	usleep(37000);
}

//Fetches then displays the fetched data in terminal
void HIH8120::displayData(){
	getMeasurement();

	cout << "humidity (%RH): " << this->getHumidity() << endl;
	cout << "temperature (C°): " << this->getTemperature() << endl;
}

// Borrowed from the ADXL345 implementation
/**
 * Method to combine two 8-bit registers into a single short, which is 16-bits on the BBB. It shifts
 * the MSB 8-bits to the left and then ORs the result with the LSB.
 * @param msb an unsigned character that contains the most significant byte
 * @param lsb an unsigned character that contains the least significant byte
 */
short HIH8120::combineRegisters(unsigned char msb, unsigned char lsb){
   //shift the MSB left by 8 bits and OR with LSB
   return ((short)msb<<8)|(short)lsb;
}

HIH8120::~HIH8120() {}

}
