#ifndef _UTIL_H
#define _UTIL_H

#include <vector>
#include <string>

std::vector<std::string> split(const std::string &s, char delim, bool skip_empties = true);

#define PAGE_SIZE 1024

// A comparator that compares two integers
typedef bool (*comparator)(int, int);

#endif
