#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <thread>
#include <sstream>
#include <iterator>

#include "trim.h"


using namespace std;

template <class Container>
void split2(const std::string& str, Container& cont, char delim = ' ')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}


float cpu_loadavg(string cpufile_name="/proc/loadavg") {

	//read in cpu file
	ifstream cpufile(cpufile_name);
	string line;

	float secondavg;

	vector<string> avg_arr; 

	while(getline(cpufile, line)) {

		split2(line, avg_arr);
		//get 5 min load avg
		secondavg = stof(avg_arr.at(1));


	}

	return secondavg;

}

/*This monitorloop sends a warning on too high load averages
*/
void monitorloop_cpu() {

	//Get number of CPUs from OS
	unsigned int concurrentThreadsSupported = thread::hardware_concurrency();
	cout << "Found virtual cores: " << concurrentThreadsSupported << endl;
	bool already_sent_alert = false; 
	cout << "starting cpu monitoring loop.." << endl;
	//Keep monitoring 5 min load avg
	while (1) {

		//5 min loadavg
		float loadavg = cpu_loadavg();
		cout << "5 min load avg is " << loadavg << endl;
		
		// if loadavg per virtual thread more than 0.9, send alert
		if (loadavg / concurrentThreadsSupported > 0.9) {

			if (!already_sent_alert) {
		
				bool isAlert = true;
				string message = "*ALERT:* Cpu under heavy load";
				send_msg(message, isAlert);
				already_sent_alert = true;	
		
			}		

		} else if (loadavg / concurrentThreadsSupported < 0.9 && already_sent_alert ){

			bool isAlert = false;
			string message = "*RESTORED:* Cpu load back to normal";
			send_msg(message, isAlert);
			already_sent_alert = false;	
		}

	sleep(61);	

	}
}

