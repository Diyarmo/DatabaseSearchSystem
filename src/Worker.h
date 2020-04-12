#include <iostream>
#include <unistd.h>
#include <string>
#include <string.h>
#include <vector>
#include "Util.h"
#include <sys/stat.h> // mkfifo
#include <fcntl.h> // O_WRONLY, O_RDONLY



class Worker
{
private:
    STRINGS filenames;
    STRINGS fields;
    STRINGS values;
    INTS fieldsIndex;
    int fifoFd;

public:
    Worker(int pipe);
    void searchFiles();
};
