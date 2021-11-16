# Filename - LEDRead.sh
# Author - Aron Kjærgaard, Kiril Iliev, Martin Tsvetkov
# Date - 16/11/2021

# Name - LEDRead.sh
# Purpose - To read the duty_cycle value in order to indicate how bright the artificial light is
# Description of inputs - There are none
# Description of return values - Returns the duty_cycle for the artificial light LED - 0 - 2ms

filepath="/sys/class/pwm/pwmchip1/pwm-1:0/duty_cycle"

while IFS= read -r line;
do
	echo "$line"
done < "$filepath"
