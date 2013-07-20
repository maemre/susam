#include "record.h"
#include "table.h"
#include <tuple>
#include <algorithm>
#include <cstring>

using namespace std;

Record::Record(const Table &t, const array<char, PAGE_SIZE> &page, int &offset)
    : values(), table(t)
{
    string fname;
    int fsize;
    auto it_begin = page.begin() + offset;
    
    for (const auto & field : table.fields) {
        tie(fname, fsize) = field;
        
        string buffer;
        buffer.resize(fsize);
        // Copy necessary amount of data
        copy(it_begin, it_begin + fsize, buffer.begin());
        // Shrink string by removing extra null characters
        buffer = buffer.substr(0, strlen(buffer.c_str()));
        // Add newly created value
        values[fname] = buffer;
        
        it_begin += fsize;
        offset += fsize;
    }
}

Record::Record(const Table &t, const vector<string> &v)
    : table(t)
{
    if (t.fields.size() != v.size())
        throw runtime_error("Record::Record: Size of values mismatch size of fields.");
        
    string fname;
    int fsize;
    int i = 0;
    
    for (const auto & field : table.fields) {
        tie(fname, fsize) = field;
        
        if (v[i].size() <= fsize)
            values[fname] = v[i];
        else {
            throw runtime_error("Record::Record: Value's size doesn't fit field size");
        }
        
        i++;
    }
}

void fill(array<char, PAGE_SIZE> &page, const char c, int begin, int end)
{
    for (int i = begin; i < end; i++)
        page[i] = c;
}

int Record::write(array<char, PAGE_SIZE> &page, int offset)
{
    string fname;
    int fsize;
    auto it_begin = page.begin() + offset;
    int offset_prev = offset;
    
    for (const auto & field : table.fields) {
        tie(fname, fsize) = field;
        
        copy(values[fname].begin(), values[fname].end(), it_begin);
        fill(page, '\0', offset + values[fname].size(), offset + fsize);
        
        it_begin += fsize;
        offset += fsize;
    }
    
    return offset - offset_prev;
}

ostream &operator << (ostream &os, const Record &r)
{
    string field = r.table.fname(0);
    os << "(" << r.values.at(field);
    
    // Cannot use modern loops, thanks to fencepost problem
    for (int i = 1; i < r.table.fields.size(); i++) {
        field = r.table.fname(i);
        os << ", " << r.values.at(field);
    }
    
    os << ")";
    return os;
}
