#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <filesystem>

namespace fs = filesystem;
using namespace std;

/*Checks if the file exists
*/
inline bool is_exists (const std::string& name) {
  struct stat buffer;

  return (stat (name.c_str(), &buffer) == 0);
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


/* A loop to monitor a url
*/
void monitorloop_sshfs(string path ) {

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

					bool isAlert = false;
					string message = "*RESTORED*: Connection restored for `" + fullpath + "`";
                                        send_msg(message, isAlert );
                                        already_notified = false;
					cout << "Restore notification sent\n";

                                }

                        }

                }

                if (connection_ok == false && already_notified == false) {

                        //the file was not found, the connection is not up
                        bool isAlert = true;
			string message = "*ALERT*: connection for `" + fullpath + "` dropped, remounting required";
                        send_msg(message, isAlert );
                        already_notified = true;
			cout << "Alert sent\n";

                }

                connection_ok = false;

                //now sleep until next check
                sleep(timeout);

        }

}



