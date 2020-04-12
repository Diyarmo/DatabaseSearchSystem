#include "Util.h"

string clearSpaces(string str){
    replace(str.begin(), str.end(), '=', '-');
    while (str.find(" -") != -1)
        str = str.replace(str.find(" -"), 2, "-");
    while (str.find("- ") != -1)
        str = str.replace(str.find("- "), 2, "-");
    return str;
}

STRINGS splitString(string str, char c)
{
    STRINGS strings;
    int pos=0;
    while(str.find(c, pos) != -1)
    {
        strings.push_back(str.substr(pos, str.find(c, pos) - pos));
        pos = str.find(c, pos) + 1;
    }
    strings.push_back(str.substr(pos));
    return strings;

}
