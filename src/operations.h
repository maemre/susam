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
void insertRecord(const Table &table, const std::vector<std::string> values);
void updateRecord(const Table &table, const std::vector<std::string> values);
void deleteRecord(const Table &table, const std::string &pk_value);
void searchRecords(const Table &table, comparator c, const std::string &pk_value);
void listAllRecords(const Table &table);


#endif // _TABLEOPS_H
