#include <iostream>
#include "Worker.h"
#include "time.h"
#include <sstream>
#include <fstream>

using namespace std;


Worker::Worker(int pipeFd)
{
    char buff[BUFF_SIZE];
    read(pipeFd, buff, BUFF_SIZE);
    string str = "";
    stringstream ss(buff);
    while (true)
    {
        getline(ss, str);
        if (str == END_OF_FILES)
            break;
        filenames.push_back(str);
        
    }
    while (getline(ss, str))
    {
        int pos = str.find('-');
        fields.push_back(str.substr(0, pos));
        values.push_back(str.substr(pos+1));
    }
    string fifoPath = WP_FIFO_PATH + to_string(getpid());
    mkfifo(fifoPath.c_str(), 0666);
    if ((fifoFd = open(fifoPath.c_str(), O_WRONLY)) < 0)
        cerr << "fifo was not created" << endl;
        
}



void Worker::searchFiles()
{
    string str;
    char buff[BUFF_SIZE];
    bool formatSent = false;
    for (int i=0; i<filenames.size(); i++)
    {
        ifstream file(filenames[i].c_str());
        getline(file, str);
        str = clearSpaces(str);
        STRINGS strings = splitString(str, '-');
        fieldsIndex.clear();
        for (int i=0; i<fields.size(); i++)
            for (int j=0; j<strings.size(); j++)
                if (fields[i] == strings[j])
                    fieldsIndex.push_back(j);
        if (!formatSent)
        {
            write(fifoFd, (str+'\n').c_str(), (str+'\n').size());
            formatSent = true;
        }
        while(getline(file, str))
        {
            str = clearSpaces(str);
            STRINGS strings = splitString(str, '-');
            bool found = true;
            for (int i=0; i<values.size(); i++)
                if (values[i] != strings[fieldsIndex[i]])
                    found = false;
            if (found)
                write(fifoFd, (str+'\n').c_str(), (str+'\n').size());}

        file.close();
    }
    if (write(fifoFd, (END_OF_ENTRIES), strlen(END_OF_ENTRIES)) < 0)
        cerr << "write to presenter pipe unsuccessful!" << endl;
    close(fifoFd);
}

int main(int argc, char** argv)
{
    Worker worker(stoi(argv[1]));
    worker.searchFiles();
}