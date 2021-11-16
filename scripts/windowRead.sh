# Filename - windowRead.sh
# Author - Aron Kjærgaard, Kiril Iliev, Martin Tsvetkov
# Date - 16/11/2021

# Name - windowRead.sh
# Purpose - To read the duty_cycle value in order to indicate in what position the servo is
# Description of inputs - There are none
# Description of return values - Returns the duty_cycle for the servo - 1ms is closed while 2ms is open

filepath="/sys/class/pwm/pwmchip1/pwm-1:1/duty_cycle"

while IFS= read -r line;
do
	echo "$line"
done < "$filepath"
