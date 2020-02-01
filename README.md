# Slack server alerts
Monitor your server using Slack - either a VM or a physical server, this app will let you know if:

Ram usage exceeded 75%
Ram usage exceeded 85%
CPU loadavg/num_cpus > 1 (loadavg also takes into account swap usage, etc)
If any of your sshfs mounted directories becomes inaccessible

## Dependencies
gcc
c++17
curl
openmp
cmake

## Installing
Run the install.sh script with sudo access. This will:

Create a /etc/slack_watchdog folder
Create the initial config files: paths.txt(sshfs mounted dirs you want to monitor), API_URL.txt(The slack webhook url path)
Add the slack_watchdog systemctl service.

## Usage
After putting your Slack webhook url into API_URL.txt, putting your wanted sshfs paths into paths.txt, you can do:
```
sudo systemctl start slack_watchdog.service

```

If things are working fine, you can enable the service so that it starts up on every startup:
```
sudo systemctl enable slack_watchdog.service

```
