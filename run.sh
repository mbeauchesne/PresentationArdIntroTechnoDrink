

TZ_adjust=-4;d=$(date +%s);t=$(echo "60*60*$TZ_adjust/1" | bc);echo T$(echo $d+$t | bc ) > /dev/ttyACM0
TZ_adjust=0;d=$(date +%s);t=$(echo "60*60*$TZ_adjust/1" | bc);echo T$(echo $d+$t | bc )

while true; do netcat 192.168.5.111 9200 < /dev/ttyACM0 ;done


https://mbjolin.ca/kibana
https://mbjolin.ca/app/kibana#/dashboard/Pour-montrer?_g=(refreshInterval:(display:'5%20seconds',pause:!f,section:1,value:5000),time:(from:now-15m,mode:quick,to:now))&_a=(filters:!(),options:(darkTheme:!f),panels:!((col:1,id:ShowTechno,panelIndex:1,row:1,size_x:12,size_y:6,type:visualization)),query:(query_string:(analyze_wildcard:!t,query:'*')),title:'Pour%20montrer',uiState:())

https://mbjolin.ca/elasticsearch
https://mbjolin.ca/elasticsearch/_plugin/hq/