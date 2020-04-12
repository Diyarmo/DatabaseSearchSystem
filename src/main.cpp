#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h> 
#define BALANCER_EXEC_PATH "bin/LoadBalancer"
using namespace std;

int main()
{
    string command;
    char * argv[3];
    argv[0] = (char*) BALANCER_EXEC_PATH;
    argv[2] = NULL;
    int pid;
    while (true)
    {
        getline(cin, command);
        if (command == "quit")
            break;
        pid = fork();        
        if (pid == 0)
        {
            argv[1] = (char*) command.c_str();
            execv(argv[0], argv);
        }
        else
            wait(NULL);
    }
}