###############################################################################
#
# This script compiles the project and sets it up as a systemd service
#
###############################################################################

#check if the user is root
uid=$(whoami)

if [ "$uid" != "root" ]; then

	echo "Please run the script as root"
	exit

fi

#compile project
clear
mkdir build/
cp src/CMakeLists.txt build/
cd build/
g++ ../src/server_monitor.cpp -Wall -fopenmp -lstdc++fs -pthread -o slack_monitor 
echo "Done compiling project"

#ask user for the slack webhook url
echo "Please generate and paste your Slack webhook url here - a hello world message will be sent out."
read webhook_url

#test the url with a simple request
curl -X POST -H 'Content-type: application/json' --data '{"text":"Hello, World!"}' $webhook_url
echo "Did you get a hello world notification to your target channel?(y/n)"
read notif_ans

if [ "$notif_ans" == "n" ]; then

	echo "Please re-enter or regenerate your Slack webhook url" 
	exit

fi

#if the token works, put it into a file for the script to read
mkdir /etc/slack_monitor
echo $webhook_url > /etc/slack_monitor/API_URL.txt 
echo "Saved webhook url"

#now copy the actual executable where systemd can find it
cp slack_monitor /etc/slack_monitor/slack_monitor

#now copy the systemctl file to the correct place
cp ../systemd/slack_monitor.service /etc/systemd/system/
echo "Added app as a systemd service"

echo "The installation is finished. Try starting the app with sudo systemctl start slack_monitor. You can enable the app on startup by doing sudo systemctl enable slack_monitor."



