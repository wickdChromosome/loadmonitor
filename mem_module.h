#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm> 
#include <cctype>
#include <locale>

#include "trim.h"

using namespace std;

/*
// trim from both ends (in place)
static inline void trim(std::string &str) {

std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
str.erase(end_pos, str.end());


}

*/


float percent_available_mem() {

	//read in mem file
	ifstream memfile("/proc/meminfo");
	string line;
	string delim = ":";

	int delim_pos;
	double memavail;
	double memtotal;
	string memtotal_kb;
	string memavail_kb;

	while(getline(memfile, line)) {

		delim_pos = line.find(delim);
		string datatype = line.substr(0, delim_pos); // token is "scott

		//find the MemTotal line first
		if (datatype == "MemTotal") {

			memtotal_kb = line.substr(line.find(delim) + 1, line.length());
			memtotal_kb[memtotal_kb.find('B')] = ' ';
			memtotal_kb[memtotal_kb.find('k')] = ' ';
			
			trim(memtotal_kb);

			memtotal = stod(memtotal_kb);


		}

		//find the MemAvailable line now
		if (datatype == "MemAvailable") {

			string memavail_kb = line.substr(line.find(delim) + 1, line.length());
			memavail_kb[memavail_kb.find('B')] = ' ';
			memavail_kb[memavail_kb.find('k')] = ' ';
			trim(memavail_kb);
			memavail = stod(memavail_kb);


		}


	}

	cout << "Total memory is " << memtotal << endl;
	cout << "Total memory  available is " << memavail << endl;
	//now lets get the percentage
	double percent_mem;
	percent_mem = memavail/memtotal;
	return percent_mem; 


}


/*This monitorloop sends a warning on too high mem consumption
*/
void monitorloop_mem() {

	cout << "starting memory monitoring loop.." << endl;
	//Keep monitoring free memory every sec
	bool already_notified_warning = false;
	bool already_notified_alert = false;
	while (1) {

		//calculate available memory percentage
		float percentmem = percent_available_mem();

		cout << "Percentage memory available is " << percentmem << endl;

		//if percentage is less than 30%, send warning
		if (percentmem < 0.3) {

			//if slack wasn't notified yet	
			if (~already_notified_warning) {
	
				bool isAlert = false;
				string message = "*WARNING:* Over 70% of ram currently in use";
			
				send_msg(message, isAlert);
				already_notified_warning = true;
			
			}

		} else if (percentmem > 0.3 && already_notified_warning){

			bool isAlert = false;
			string message = "*RESTORED:* Ram usage under 70%";
			send_msg(message, isAlert);
			already_notified_warning = false;
		
		}
		

		// if percentage is more than 85%, send alert
		if (percentmem < 0.15) {

			bool isAlert = true;
			string message = "*ALERT:* Over 85% of ram currently in use";

			//if slack wasn't notified yet	
			if (~already_notified_alert) {
			
				send_msg(message, isAlert);
				already_notified_alert = true;


			}	
	

		} else if (percentmem > 0.15 && already_notified_alert) {

			bool isAlert = false;
			string message = "*RESTORED:* Ram usage under 85%";
			send_msg(message, isAlert);
			already_notified_alert = false;

		}

	sleep(1);	

	}
}

