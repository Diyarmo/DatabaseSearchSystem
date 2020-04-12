#include <fcntl.h> // O_WRONLY, O_RDONLY
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <algorithm>    // std::sort
#include "Util.h"

using namespace std;

class Presenter
{
private:
    int sort ;
    string sortField;
    int sortFieldIndex;
    STRINGS fifoPaths;
    vector <STRINGS> results;
public:
    Presenter();
    void readFifos();
    void sortResults();
    void showResults();
    bool operator () (STRINGS a, STRINGS b);

};

