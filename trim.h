#pragma once

#include <iostream>
#include <string>


using namespace std;


// trim from both ends (in place)
static inline void trim(std::string &str) {

std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
str.erase(end_pos, str.end());


}


