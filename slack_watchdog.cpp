#include <iostream>
#include <string>
#include <vector>

//slack webhook messaging
#include "messaging.h"

//monitoring modules
#include "sshfs_module.h"
#include "mem_module.h"
#include "cpu_module.h"

using namespace std;
namespace fs = filesystem;

int main(int argc, char* argv[]) {

	string confpath = argv[1];

	vector<string> paths2monitor = parse_config (confpath); 

	cout << "initializing openMP threads.." << endl;
	cout << "Found paths in config file: " << paths2monitor.size() << endl;	
	//lets fire up some openMP threads
	#pragma omp parallel for
	for (int i = 0; i < paths2monitor.size() + 2 ; i++) {

		if (i == paths2monitor.size()) monitorloop_mem();	
		if (i == paths2monitor.size() + 1) monitorloop_cpu();	
		if (i < paths2monitor.size()) {

			 monitorloop_sshfs( paths2monitor[i] );
		
			//sshfs monitoring module
			cout << "starting sshfs monitor thread for " << paths2monitor[i] << endl;


		}
	}
		
	return 0;


}
