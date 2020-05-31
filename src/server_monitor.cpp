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


	// Lets create a separate process the memory and cpu loops
	if (fork() == 0) monitorloop_mem();	
	else monitorloop_cpu();	
		
	return 0;


}
