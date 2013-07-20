#ifndef _TABLEOPS_H
#define _TABLEOPS_H

#include <vector>
#include <utility>
#include <string>
#include "table.h"
#include "util.h"

// Initialize manager objects
void init_managers();

// Table operations:
void listAllTables();
void createTable(const Table &table);
void deleteTable(const std::string &name);

// Record operations:
// Side note: do all checks in insert record
void insertRecord(const std::string &name, const std::vector<std::string> values);
void updateRecord(const std::string &name, const std::vector<std::string> values);
void deleteRecord(const std::string &name, const std::string &pk_value);
void searchRecords(const std::string &name, comparator c, const std::string &pk_value);
void listAllRecords(const std::string &name);


#endif // _TABLEOPS_H
