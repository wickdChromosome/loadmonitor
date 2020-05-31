#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

//slack webhook messaging
#include "messaging.h"

//monitoring modules
#include "mem_module.h"
#include "cpu_module.h"

using namespace std;

int main(int argc, char* argv[]) {

	cout << "initializing openMP threads.." << endl;
	//lets fire up some openMP threads
	#pragma omp parallel for
	for (int i = 0; i < 2 ; i++) {

		if (i == 0) monitorloop_mem();	
		if (i == 1) monitorloop_cpu();	

	}
		
	return 0;


}
