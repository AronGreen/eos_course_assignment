cd /sys/class/gpio/gpio115
echo in > direction

cd /sys/bus/iio/devices/iio\:device0
cat in_voltage0_raw
