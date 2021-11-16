cd /sys/class/gpio/gpio49
echo out > direction
echo "$1" > value
