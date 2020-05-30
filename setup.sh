###############################################################################
#
# This script compiles the project and sets up a systemctl service
#
###############################################################################

#compile project
clear
mkdir build/
cp src/CMakeLists.txt build/
cd build/
cmake CMakeLists.txt 
make

#ask user for the slack token


#test the token with a simple request


#if the token works, make 
