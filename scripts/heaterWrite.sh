# Filename - heaterWrite.sh
# Author - Aron Kjærgaard, Kiril Iliev, Martin Tsvetkov
# Date - 16/11/2021

# Name - heaterWrite.sh
# Purpose - To initiate and switch the heater on/off
# Description of inputs - A 0 to indicate off and 1 or any other positive integer for on
# Description of return values - There are none

cd /sys/class/gpio/gpio49
echo out > direction
echo "$1" > value
