#include "operations.h"
#include "syscat.h"
#include <string>
#include <iostream>
#include <cctype>

using namespace std;

SystemCatalogue syscat;

void init_managers()
{
    syscat.open();
}

void deleteTable(const string &name)
{
    if (name.empty()) {
        cout << "Table name cannot be empty." << endl;
        return;
    }
    for (auto c : name) {
        if (c != '\0' && c!= '-' && c!='_' && !isalnum(c)) {
            cout << "Table name must contain alphanumeric characters or '-' or '_' only." << endl;
            return;
        }
    }
    if (syscat.deleteTable(name))
        cout << "Table '" << name << "' deleted successfully." << endl;
    else
        cout << "Table '" << name << "' could not be deleted since it does not exist." << endl;
}

void createTable(const Table &t)
{
    if (syscat.createTable(t))
        cout << "Table '" << t.name << "' created successfully." << endl;
    else
        cout << "Table '" << t.name << "' could not be created since it already exists." << endl;
}

void listAllTables()
{
    syscat.listAllTables();
}
