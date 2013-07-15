#ifndef _TABLE_H
#define _TABLE_H

#include <vector>
#include <utility>
#include <string>
#include <ostream>

struct Table {
    std::vector<std::pair<std::string,int> > fields;
    std::string name;
    // Index of primary key
    int pk_index;
    
    int &fsize(int index);
    std::string &fname(int index);
    // Immutable getters
    const int &fsize(int index) const;
    const std::string &fname(int index) const;
};

std::ostream & operator << (std::ostream &os, const Table &t);

#endif
