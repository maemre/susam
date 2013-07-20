#include "util.h"
#include <sstream>

using namespace std;

/*
 * Split the string by delimiters, skip consecutive delimiters
 * if skip_empties is true.
 *
 * \param s the string to be parsed
 * \param delim the delimiter character
 * \param skip_empties skip empty strings
 * \return the vector of parsed strings
 */
std::vector<string> split(const string &s, char delim, bool skip_empties)
{
    std::vector<string> v;
    std::stringstream ss(s);
    std::string item;
    
    while (std::getline(ss, item, delim)) {
        if (skip_empties && item == "")
            continue;
            
        v.push_back(item);
    }
    
    return v;
}

// trim leading spaces
void triml(string &str)
{
    size_t startpos = str.find_first_not_of(" \t");
    
    if (string::npos != startpos)
        str = str.substr(startpos);
}

// trim following spaces
void trimr(string &str)
{
    size_t endpos = str.find_last_not_of(" \t");
    
    if (string::npos != endpos)
        str = str.substr(0, endpos + 1);
}

void trim(string &str)
{
    trimr(str);
    triml(str);
}

int bytesToInt(char *bytes, int offset)
{
    int r = 0;
    
    for (int i = 0; i < 4; i++) {
        r <<= 8; // shift by one byte
        r += (unsigned char)bytes[offset + i];
    }
    
    return r;
}

// TODO: Fix it if necessary
void intToBytes(char *bytes, int offset, int n)
{
    for (int i = 0; i < 4; i++) {
        bytes[offset + i] = (char)n;
        n >>= 8; // shift by one byte
    }
}
