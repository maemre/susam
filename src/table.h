#ifndef _TABLE_H
#define _TABLE_H

#include <vector>
#include <utility>
#include <string>
#include <ostream>

typedef std::pair<std::string, int> field_t;

struct Table {
    bool isDeleted;
    std::vector<field_t> fields;
    std::string name;
    // Index of primary key
    int pk_index;
    
    int &fsize(int index);
    std::string &fname(int index);
    // Immutable getters
    const int &fsize(int index) const;
    const std::string &fname(int index) const;
    // Add move semantics and copy semantics
    Table(const Table &other); // copy
    Table(Table &&other); // move
    Table(); // regular constructor
    
    int recordSize();
    
    Table &operator = (Table &other);
    Table &operator = (Table && other);
};

std::ostream &operator << (std::ostream &os, const Table &t);

#endif
