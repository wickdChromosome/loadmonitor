#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <string>
#include <iostream>

//The modules to be tested
#include "../src/messaging.h"
#include "../src/mem_module.h"
#include "../src/cpu_module.h"


TEST_CASE("Test for module parsing", "[classic]")
{

	SECTION("Test memory file parsing"){
		std::string testmempath = "test_mem.txt";
		REQUIRE(to_string(percent_available_mem(testmempath)) == "0.482680");
	}
			
			
	SECTION("Test load avg file parsing"){	
		std::string testcpupath = "test_loadavg.txt";
		REQUIRE(to_string(cpu_loadavg(testcpupath)) == "0.770000");
	}

}
