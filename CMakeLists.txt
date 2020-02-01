cmake_minimum_required(VERSION 3.10)
project(slack_watchdog)

#out execs
add_executable(slack_watchdog slack_watchdog.cpp)
add_executable(tests tests.cpp)

#Whew lad
#link OpenMP
if(NOT TARGET OpenMP::OpenMP_CXX)
	
	find_package(Threads REQUIRED)
	add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
	set_property(TARGET OpenMP::OpenMP_CXX 
		PROPERTY INTERFACE_COMPILE_OPTIONS ${OpenMP_CXX_FLAGS})
	# Only works if the same flag is passed to the linker; use CMake 3.9+ otherwise (Intel, AppleClang)
	set_property(TARGET OpenMP::OpenMP_CXX
	       	PROPERTY INTERFACE_LINK_LIBRARIES ${OpenMP_CXX_FLAGS} Threads::Threads)
endif()

target_link_libraries(slack_watchdog PUBLIC OpenMP::OpenMP_CXX)


add_definitions("-Wall -lstdc++fs -pthread -std=c++17 ")


