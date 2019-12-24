API_KEY='https://hooks.slack.com/services/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'

templines=$(sudo sensors | grep 'Tdie' | cut -d' ' -f10 | sed 's/°C//g')
FS=', ' read -r -a linearray <<< "$templines"
temphighs=$(sudo sensors | grep 'Tdie' | cut -d' ' -f14 | sed -e 's/°C)//g')
FS=', ' read -r -a higharray <<< "$temphighs"
temp_high_counter=0

for temp in "${linearray[@]}"; do


	temp1=$(echo $temp | cut -d. -f1)
	temp_high=${higharray[temp_high_counter]} 
	temp_high1=$(echo $temp_high | cut -d. -f1)
	#cut off decimal part to make life easier
	#echo "high: " $temp_high1
	#echo "current: " $temp1
	if ((temp1 > temp_high1))
	then

		curl -X POST -H 'Content-type: application/json' --data '{"text":"*ALERT:* Server device temperature exceeded threshold!!"}' $API_KEY
	
	fi

	temp_high_counter=temp_high_counter+1

	sleep 5

done


