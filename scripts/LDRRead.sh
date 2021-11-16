# Filename - LDRRead.sh
# Author - Aron Kjærgaard, Kiril Iliev, Martin Tsvetkov
# Date - 16/11/2021

# Name - LDRRead.sh
# Purpose - To read the raw voltage of the LDR
# Description of inputs - There are none
# Description of return values - Returns the value of the LDR - for this project we decided to say that anything below 1000
# is considered 0% light and 3500 is considered 100%

cd /sys/class/gpio/gpio115
echo in > direction

cd /sys/bus/iio/devices/iio\:device0
cat in_voltage0_raw
