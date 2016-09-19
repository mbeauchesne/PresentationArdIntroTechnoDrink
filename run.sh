

TZ_adjust=-4;d=$(date +%s);t=$(echo "60*60*$TZ_adjust/1" | bc);echo T$(echo $d+$t | bc ) > /dev/ttyACM0

while true; do netcat 192.168.5.111 9200 < /dev/ttyACM0 ;done