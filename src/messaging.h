#pragma once

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

string get_API_URL() {

	//Read the API form the config path
	string config_path = "/etc/pejlab_watchdog/API_URL.txt";
	string line;
	ifstream apifile(config_path);	

	while (getline(apifile, line)) {
	
		//read just the first line, where the API key is
		if (line != "") {

			return line;

		}	


	}
	cout << "Unable to read config file\n";
	exit(EXIT_FAILURE);

}


void send_msg(string message, bool isAlert = false) {

	string api_url = get_API_URL();

	if (isAlert) {

		message = "*[!]* " + message;

	} else {

		message = "*[+]* " + message;

	}

	string curl_request = "curl -X POST -H \'Content-type: application/json\' --data \'{\"text\":\" "+ message + "\"}\' "  + api_url;

	system( curl_request.c_str() );	

}



