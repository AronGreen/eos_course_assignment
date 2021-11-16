filepath="/sys/class/pwm/pwmchip1/pwm-1:0/duty_cycle"

while IFS= read -r line;
do
	echo "$line"
done < "$filepath"
