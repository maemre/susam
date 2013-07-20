#ifndef _UTIL_H
#define _UTIL_H

#include <vector>
#include <string>

std::vector<std::string> split(const std::string &s, char delim, bool skip_empties = true);

#define PAGE_SIZE 1024

// A comparator that compares two integers
typedef bool (*comparator)(int, int);

// Right trim a string in-place
void trimr(std::string &str);
// Left trim a string in-place
void triml(std::string &str);
// Trim a string in-place
void trim(std::string &str);

int bytesToInt(char *bytes, int offset);

void intToBytes(char *bytes, int offset, int n);

#endif
