#include "Presenter.h"

using namespace std;

Presenter::Presenter()
{
    int fd = open(FIFO_PATH, O_RDONLY);
    char buff[BUFF_SIZE];
    read(fd, buff, BUFF_SIZE);
    string str = "";
    stringstream ss(buff);
    getline(ss, str);
    if ((sort = stoi(str)) != NONE)
        getline(ss, sortField);
    
    while (getline(ss, str))       
    {
        fifoPaths.push_back(WP_FIFO_PATH + str);
    }
    close(fd);
}   
void Presenter::readFifos()
{
    char buf[BUFF_SIZE];
    int fd1;
    for (int i=0; i<fifoPaths.size(); i++)
    {
        stringstream ss1;
        ifstream file(fifoPaths[i].c_str());
        string str;
        getline(file, str);
        STRINGS fields = splitString(str, '-');
        for (int i=0; i<fields.size(); i++)
            if (fields[i] == sortField)
                sortFieldIndex = i;
        while (true)
        {
            while(getline(file, str))
            {
                results.push_back(splitString(str, '-'));
            }
            if (str != END_OF_ENTRIES)
               usleep(10000);
            else
            {
                results.pop_back();
                break;
            }
        }
        file.close();
    }
}

bool Presenter::operator ()(STRINGS a, STRINGS b)
{
    if (sort == ASCENDING)
        return a[sortFieldIndex] < b[sortFieldIndex]; 
    else if (sort == DESCENDING)
        return a[sortFieldIndex] > b[sortFieldIndex]; 
    else return true;

}


void Presenter::sortResults()
{   
    std::sort(results.begin(), results.end(), *this);
}

void Presenter::showResults()
{
    for (int i=0; i<results.size(); i++)
    {
        cout << i+1 << "-\t";
        for (int j=0; j<results[i].size(); j++)
            cout << results[i][j] << " - ";
        cout << endl;
    }
    cout << "-----------------------------------------------" << endl;
}


int main()
{
    Presenter presenter;
    presenter.readFifos();
    presenter.sortResults();
    presenter.showResults();
}