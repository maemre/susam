#ifndef _RECORD_H
#define _RECORD_H

#include "util.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <array>
#include <utility>
#include <vector>
#include <ostream>
#include "table.h"

class Record {
public:
    std::unordered_map<std::string, std::string> values;
    const std::shared_ptr<Table> table;
    // constructors:
    
    // Create record with given table, whose values are
    // in page, offset with given offset. The offset
    // will be overwritten when construction is done.
    Record(const std::shared_ptr<Table> &t, const std::array<char, PAGE_SIZE> &page, int &offset);

    // Create a record using values from v
    Record(const std::shared_ptr<Table> &t, const std::vector<std::string> &v);

    // Write the record to a page with some offset
    // Return number of bytes written
    int write(std::array<char, PAGE_SIZE> &page, int offset);
};

// For pretty printing
std::ostream & operator << (std::ostream &os, const Record &r);

#endif
