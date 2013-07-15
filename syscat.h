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
    bool insertTable(const Table &t);
    bool deleteTable(const std::string &name);
    
    SystemCatalogue();
    ~SystemCatalogue();
private:
    std::vector<std::string> linesRead_;
    int pos_;
    std::fstream fs;
};

#endif
