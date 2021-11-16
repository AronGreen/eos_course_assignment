# Filename - heaterRead.sh
# Author - Aron Kjærgaard, Kiril Iliev, Martin Tsvetkov
# Date - 16/11/2021

# Name - heaterRead.sh
# Purpose - To read the state value of the "heater" LED in order to indicate on/off state
# Description of inputs - There are none
# Description of return values - Returns the state value of the "heater" LED - 0 for off, 1 for on

cd /sys/class/gpio/gpio49
cat value
