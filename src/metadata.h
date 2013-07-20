#ifndef _METADATA_H
#define _METADATA_H

#include "util.h"
#include "table.h"
#include <fstream>
#include <stdexcept>

class Metadata {
public:
    Metadata(const Table &t);
    ~Metadata();
    int nextPageId();
    void bindNewPage(int pageId);
private:
    std::fstream fs;
    std::vector<int> pageIdBuffer;
    Table t_;
    void fillBuffer();
    char *buffer;
};

#endif // _METADATA_H
