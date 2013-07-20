#include "datapage.h"

using namespace std;

DataPage::DataPage()
    : state(State::not_used), records(), table()
{
}

DataPage::DataPage(const array<char, PAGE_SIZE> &page, const Table &t)
    : records(), table(t)
{
    state = (State::state_t)page[0]; // Read state directly
    records.reserve(page[1]); // Number of records
    
    int offset = 2;
    
    for (int i = 0; i < page[1]; ++i) {
        Record r(t, page, offset); // record automatically increases offsets
        records.push_back(r);
    }
}

bool DataPage::addRecord(const Record &r)
{
    if (state == State::full)
        return false;
        
    records.push_back(r);
    
    if ((records.size() + 1)*table.recordSize() > PAGE_SIZE)
        state = State::full;
        
    return true;
}

bool DataPage::write(array<char, PAGE_SIZE> &page)
{

    if (records.size()*table.recordSize() > PAGE_SIZE)
        return false;
        
    page[0] = (char)state;
    page[1] = records.size();
    
    int offset = 2;
    
    for (Record & r : records) {
        offset += r.write(page, offset);
    }
    
    return true;
}
