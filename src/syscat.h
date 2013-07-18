#ifndef _SYSCAT_H
#define _SYSCAT_H

#include <fstream>
#include <vector>
#include <string>
#include "table.h"

class SystemCatalogue {
public:
    void listAllTables();
    Table findTable(const std::string &name);
    bool createTable(const Table &t);
    bool deleteTable(const std::string &name);
    
    SystemCatalogue();
    ~SystemCatalogue();
    
    void open();
private:
    std::vector<std::string> linesRead_;
    int pos_;
    std::fstream fs;
};

#endif
