###############################################################################
#
# This script compiles the project and sets up a systemctl service
#
###############################################################################

#compile project
clear
mkdir build/
cp src/CMakeLists.txt build/
cd build/
cmake CMakeLists.txt 
make

#ask user for the slack webhook url
echo "Please generate and paste your Slack webhook url here"
read webhook_url

#test the url with a simple request
curl -X POST -H 'Content-type: application/json' --data '{"text":"Hello, World!"}' $webhook_url
echo "Did you get a notification to your target channel?(y/n)"
read notif_ans

#if the token works, put it into a file for the script to read
mkdir /etc/slack_monitor
echo $webhook_url > /etc/slack_monitor/API_URL.txt 

#now copy the systemctl file there
cp systemd/slack_monitor.service /etc/systemd/system/

#now load the systemctl file




