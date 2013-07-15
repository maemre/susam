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
std::vector<string> split(const string &s, char delim, bool skip_empties) {
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

