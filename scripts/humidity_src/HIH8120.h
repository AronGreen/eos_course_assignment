/*
    Filename - HIH8120.h
    Author - Aron Kjærgaard, Kiril Iliev, Martin Tsvetkov
    Date - 16/11/2021
*/

#ifndef HIH8120_H_
#define HIH8120_H_

#include "I2CDevice.h"

namespace I2C_HIH8120 {

class HIH8120: protected I2CDevice {
public:
	HIH8120(unsigned int I2CBus, unsigned int I2CAddress=0x53);

	virtual int getMeasurement();
	virtual void displayData();
	virtual float getHumidity();
	virtual float getTemperature();
	virtual ~HIH8120();

private:
	unsigned int I2CBus, I2CAddress;
	short humidity, temperature;
	virtual void prepareMeasurement();
	short combineRegisters(unsigned char msb, unsigned char lsb);
};

}

#endif /* HIH8120_H_ */


