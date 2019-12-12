#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>


using namespace std;
namespace fs = filesystem;

/*Checks if the file exists
*/
inline bool is_exists (const std::string& name) {
  struct stat buffer;   

  return (stat (name.c_str(), &buffer) == 0); 
}

/*Parses config file to get list of dirs to monitor
*/
vector<string> parse_config (string confpath) {
	
		ifstream conffs(confpath);
		string monitor_path; //holds url to monitor from file + file to monitor
		vector<string> monitor_paths;

		while (std::getline(conffs, monitor_path)) {
	    	
			if (monitor_path == "") continue;
					
			monitor_paths.push_back(monitor_path);

		}
		
		conffs.close();

		return monitor_paths;


}

/* Sends a message to the Slack app using curl
*/
void send_alert_message(string fullpath) {

	string api_url = "https://hooks.slack.com/services/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";
	string message_string = "ALERT: sshfs connection interrupted, remounting required for `" + fullpath + "`";
	string curl_request = "curl -X POST -H \'Content-type: application/json\' --data \'{\"text\":\" "+ message_string + "\"}\' "  + api_url;

	system( curl_request.c_str() );	

	//cout << curl_request << endl; 
	cout << "ALERT sent" << endl; 

}

void send_recovery_message(string fullpath) {

	string api_url = "https://hooks.slack.com/services/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";
	string message_string = "INFO: sshfs connection restored for `" + fullpath + "`";
	string curl_request = "curl -X POST -H \'Content-type: application/json\' --data \'{\"text\":\" "+ message_string + "\"}\' "  + api_url;

	system( curl_request.c_str() );	

	//cout << curl_request << endl; 
	cout << "RESTORE message sent" << endl; 

}

/*Create a dotfile to recognize whether dir is mounted
*/
string init(string path) {

	//if the directory ends with #, user wants us to create a monitoring file for them
	if (path.back() == '#') {

		path = path + "/.sshfs_monitor";
		cout << "initialising hidden file " << path << endl;
		
		//check if file exists first
		bool file_isexists = is_exists(path);

		//if the file does not exist yet
		if (file_isexists == 0) {
		
			cout << "Dot file does not exist yet, initializing.." << endl;	
			//write a dotfile to recognize connection to dir

			cout << "Writing file: " << path << endl;	
			std::ofstream ofs(path);
			ofs << "This file was created by sshfs_monitor to keep track of this connection\n"; 
			ofs.close();

		}


	} 

	//if there was no # at the end of the string, 
	return path;

}

/* A loop to monitor a url
*/
void monitorloop(string path ) {

	int timeout = 2; //in seconds
	bool connection_ok = false;
	bool already_notified = false;

	//init all of these monitoring paths
	string fullpath = init(path);

	//if path does not end with '#' (indicating that we should make a file)
	//then cut off last part of string to get real path

	//lets make sure that the dir doesn't end with '/' by mistake
	if (path.back() == '/') path.pop_back();

	//cut off last part of the input path, to get a dir we can traverse
	//to find the file/dir we want to monitor (fullpath)
	size_t last_slash_pos = path.find_last_of("/");
	path = path.erase(last_slash_pos, path.length());
	cout << "Base path was found to be: " << path << endl;
	cout << "fullpath was found to be: " << fullpath << endl;


	//infinite loop that checks for the file by the specified time interval	
	while( 1 ) {

		//check if the directory is empty (if yes, the dir is unmounted)
		for (const auto & entry : fs::directory_iterator(path)) {
			
			string read_in_obj = entry.path();
			if (read_in_obj == fullpath) {

				cout << "connection OK" << endl;	
				cout << fullpath << endl;	
				connection_ok = true;

				//check if the connection is back up
				if (already_notified == true) {

					send_recovery_message(fullpath);
					already_notified = false;

				}
	
			}
		
		}

		if (connection_ok == false && already_notified == false) {

			//the file was not found, the connection is not up
			send_alert_message(fullpath);	
			already_notified = true;

		} 
 
		connection_ok = false;	
	
		//now sleep until next check
		sleep(timeout);	
	
	}

}

int main(int argc, char* argv[]) {

	string confpath = argv[1];

	vector<string> paths2monitor = parse_config (confpath); 

	cout << "initializing openMP threads.." << endl;
	cout << "Found paths in config file: " << paths2monitor.size() << endl;	
	//lets fire up some openMP threads
	#pragma omp parallel for
	for (int i = 0; i < paths2monitor.size(); i++) {

		cout << "starting monitor thread for " << paths2monitor[i] << endl;
		monitorloop( paths2monitor[i] );

	}
		
	return 0;


}
