#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <iostream>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>

#define WORKER_EXEC_PATH "bin/Worker"
#define PRESENTER_EXEC_PATH "bin/Presenter"
#define FIFO_PATH "namedPipe/fifo"
#define WP_FIFO_PATH "namedPipe/wpfifo"
#define NONE 0
#define ASCENDING 1
#define DESCENDING -1
#define END_OF_FILES "END_OF_FILES"
#define END_OF_ENTRIES "END_OF_ENTRIES"

#define STRINGS vector<string> 
#define INTS vector<int> 
#define P_INTS vector<int *> 

#define BUFF_SIZE 10240

using namespace std;

string clearSpaces(string str);
STRINGS splitString(string str, char c);

#endif