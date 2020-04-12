#include <string>
#include <vector>
#include "Util.h"
#include <dirent.h> // DIR 
#include <unistd.h> // fork, execv, pipe
#include <sys/stat.h> // mkfifo
#include <fcntl.h> // O_WRONLY, O_RDONLY
#include <string.h>
#include <sys/wait.h> // waitpid


using namespace std;

class LoadBalancer
{
private:

STRINGS fields;
STRINGS values;
int sort;
string sortField;
int numOfProcesses;
string directory;
void findDatabaseFiles();
void parseCommand(char * command);
STRINGS files;
P_INTS workerPipes;
INTS workerPIDs;
int presenter;

public:
    LoadBalancer(char * command);
    ~LoadBalancer();
    void initWorkers();
    void sendDataToWorkers();
    void initPresenter();
    void sendDataToPresenter();
};
