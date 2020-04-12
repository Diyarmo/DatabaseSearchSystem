#include <iostream>
#include "LoadBalancer.h"

using namespace std;
#define READFD 0
#define WRITEFD 1

LoadBalancer::LoadBalancer(char * command)
{
    sort = NONE;
    parseCommand(command);
    findDatabaseFiles();
}

LoadBalancer::~LoadBalancer()
{
    for(int i=0; i<workerPIDs.size(); i++)
        waitpid(workerPIDs[i], NULL, WNOHANG);
    waitpid(presenter, NULL, WNOHANG);
}

void LoadBalancer::parseCommand(char * command)
{
    string cmd = command, field, value;
    numOfProcesses = 1;
    int pos=0;
    cmd = clearSpaces(cmd);
    STRINGS strings = splitString(cmd, '-');
    for (int i=0; i<(strings.size()/2); i++)
    {
        field = strings[2*i];
        value = strings[2*i+1];
        if (field == "processes")
            numOfProcesses = stoi(value);
        else if (field == "dir")
            directory = value;
        else if (value == "ascending" || value == "descending" )
        {
            sort = (value == "ascending")? ASCENDING:DESCENDING;
            sortField = field;
        }
        else
        {
            fields.push_back(field);
            values.push_back(value);
        }
    }
}

void LoadBalancer::findDatabaseFiles()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (directory.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            string fileName = ent -> d_name;
            if (fileName.find("dataset") != -1)
                files.push_back(fileName);     
        }
        closedir (dir);
    }
    else
        cerr << "directory not found!" << endl;
    
}

void LoadBalancer::initWorkers()
{
    char * argv[3];
    int *fd, pid;
    argv[0] = (char *) WORKER_EXEC_PATH;
    argv[2] = NULL;
    for (int i=0; i<numOfProcesses; i++)
    {
        fd = new int[2];
        if (pipe(fd) < 0)
        {
            cerr << "pipe not created!" << endl;
            return;
        }
        workerPipes.push_back(fd);
        pid = fork();
        if (pid < 0)
        {
            cerr << "fork unsuccessful" << endl;
            return;
        }
        else if (pid == 0)
        {
            argv[1] = (char *) to_string(fd[READFD]).c_str();
            execv(argv[0], argv);
        }
        else 
            workerPIDs.push_back(pid);

    }
}

void LoadBalancer::initPresenter()
{ 
    int pid = fork();
    if(pid < 0)
        cerr << "fork presenter unsuccessful" << endl;
    else if(pid == 0)
        execv(PRESENTER_EXEC_PATH, NULL);
    else
    {   presenter = pid;
        mkfifo(FIFO_PATH, 0666);
    }
}


void LoadBalancer::sendDataToWorkers()
{
    int wid = 0;
    string filters;
    for (int i=0; i<files.size(); i++)
    {
        string filePath = (directory + "/" + files[i] + "\n");
        write(workerPipes[wid++][WRITEFD], filePath.c_str(), filePath.size());
        wid %= numOfProcesses;
    }
    for (int i=0; i<fields.size(); i++)
        filters += fields[i] + "-" + values[i] + '\n';
    for (int i=0; i<workerPipes.size(); i++)
    {
        write(workerPipes[i][WRITEFD], END_OF_FILES, strlen(END_OF_FILES));
        write(workerPipes[i][WRITEFD], "\n", 1);
        write(workerPipes[i][WRITEFD], (char *)filters.c_str(), filters.size()+1);
        close(workerPipes[i][WRITEFD]);
        close(workerPipes[i][READFD]);
    }
}

void LoadBalancer::sendDataToPresenter()
{
    string toSend;
    toSend += to_string(sort) + '\n';
    if (sort != NONE)
        toSend +=  sortField + '\n';
    for (int i=0; i<workerPIDs.size(); i++)
        toSend += to_string(workerPIDs[i]) + '\n';
    int fd = open(FIFO_PATH, O_WRONLY);
    write(fd, toSend.c_str() , strlen(toSend.c_str()) + 1);
    close(fd);    
}

int main(int argc, char **argv)
{
    LoadBalancer lb(argv[1]);
    lb.initWorkers();
    lb.sendDataToWorkers();
    lb.initPresenter();
    lb.sendDataToPresenter();
}