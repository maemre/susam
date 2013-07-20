#ifndef _DATAPAGE_H
#define _DATAPAGE_H

#include "table.h"
#include "record.h"

namespace State {
enum state_t {
    vacant = 0,
    full = 1,
    not_used = 2
};
};

class DataPage {
public:
    State::state_t state;
    std::vector<Record> records;
    Table table;
    
    DataPage(const std::array<char, PAGE_SIZE> &page, const Table &t);
    DataPage();
    
    bool addRecord(const Record &r);
    bool write(std::array<char, PAGE_SIZE> &page);
};

#endif // _DATAPAGE_H
