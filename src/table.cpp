#include "table.h"
#include <stdexcept>

using namespace std;

const int &Table::fsize(int index) const
{
    if (index >= fields.size())
        throw runtime_error("Table::fsize: given index is larger than # of fields");
        
    return fields[index].second;
}

const string &Table::fname(int index) const
{
    if (index >= fields.size())
        throw runtime_error("Table::fname: given index is larger than # of fields");
        
    return fields[index].first;
}

int &Table::fsize(int index)
{
    if (index >= fields.size())
        throw runtime_error("Table::fsize: given index is larger than # of fields");
        
    return fields[index].second;
}

string &Table::fname(int index)
{
    if (index >= fields.size())
        throw runtime_error("Table::fname: given index is larger than # of fields");
        
    return fields[index].first;
}

Table::Table()
    : isDeleted(true), fields(0), name(""), pk_index(-1)
{
}

Table::Table(Table &other)
    : isDeleted(other.isDeleted),
      fields(other.fields),
      name(other.name),
      pk_index(other.pk_index)
{
}

Table::Table(Table &&other)
    : isDeleted(other.isDeleted),
      fields(move(other.fields)),
      name(move(other.name)),
      pk_index(other.pk_index)
{
}

ostream &operator << (ostream &os, const Table &t)
{
    os << t.name << '(';
    
    if (t.pk_index == 0)
        os << "[PK] ";
        
    os << t.fname(0) << " : " << t.fsize(0);
    
    // No range based for loop, thanks to fencepost problem again
    for (int i = 1; i < t.fields.size(); i++) {
        os << ", ";
        
        if (i == t.pk_index)
            os << "[PK] ";
            
        os << t.fname(i) << " : " << t.fsize(i);
    }
    
    os << ')';
    return os;
}

Table &Table::operator = (Table &other)
{
    isDeleted = other.isDeleted;
    fields = other.fields;
    name = other.name;
    pk_index = other.pk_index;
    return *this;
}

Table &Table::operator = (Table && other)
{
    isDeleted = other.isDeleted;
    fields = move(other.fields);
    name = move(other.name);
    pk_index = other.pk_index;
    return *this;
}
