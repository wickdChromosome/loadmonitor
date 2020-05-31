cmake_minimum_required(VERSION 3.10)
project(slack_monitor)

add_executable(slack_monitor ../src/server_monitor.cpp)
target_link_libraries(slack_monitor PUBLIC OpenMP::OpenMP_CXX)


add_definitions("-Wall -fopenmp -lstdc++fs -pthread -std=c++17 ")


