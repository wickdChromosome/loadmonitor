#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <string>
#include <iostream>

//The modules to be tested
#include "messaging.h"
#include "mem_module.h"
#include "cpu_module.h"
#include "sshfs_module.h"


TEST_CASE("Test for module parsing", "[classic]")
{

	SECTION("Test memory file parsing"){
		REQUIRE(to_string(percent_available_mem("../test_mem.txt")) == "0.482680");
	}
			
			
	SECTION("Test load avg file parsing"){	
		REQUIRE(to_string(cpu_loadavg("../test_loadavg.txt")) == "0.770000");
	}

}
