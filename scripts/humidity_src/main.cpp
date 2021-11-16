
#include "HIH8120.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace I2C_HIH8120;
using namespace std;



int main(int argc, char* argv[]){
	HIH8120 s = HIH8120(0x02, 0x27);

	std::istringstream iss( argv[1] );
	string arg;

	if (iss >> arg){
		if (arg == "hum")
		{
			cout << s.getHumidity();
			return 0;
		}
		else if (arg =="temp")
		{
			cout << s.getTemperature();
			return 0;
		}
		else {
			s.displayData();
			return 0;
		}
	}
	return -1;
}
