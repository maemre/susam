#include <iostream>
#include "syscat.h"
#include <stdexcept>

using namespace std;

SystemCatalogue syscat;

void createTableTest()
{
    cout << "Creating two tables" << endl;
    Table t;
    t.isDeleted = false;
    // Table 1
    t.name = "students";
    t.pk_index = 0;
    t.fields.push_back(make_pair("student id", 10));
    t.fields.push_back(make_pair("name", 15));
    t.fields.push_back(make_pair("dept", 4));
    t.fields.push_back(make_pair("email", 100));
    cout << "1. Result: " << syscat.createTable(t) << endl;
    
    t.name = "courses";
    t.pk_index = 0;
    t.fields.clear();
    t.fields.push_back(make_pair("course id", 4));
    t.fields.push_back(make_pair("dept", 4));
    t.fields.push_back(make_pair("year", 4));
    cout << "2. Result: " << syscat.createTable(t) << endl;
}

void deleteTableTest()
{
    cout << "Deleting table students" << endl;
    syscat.deleteTable("students");
}

void listTablesTest()
{
    cout << "Listing all tables." << endl;
    syscat.listAllTables();
}

void comboTest()
{
    listTablesTest();
    deleteTableTest();
    listTablesTest();
    createTableTest();
    listTablesTest();
    cout << "Deleting all tables" << endl;
    syscat.deleteTable("courses");
    syscat.deleteTable("students");
    listTablesTest();
    createTableTest();
    listTablesTest();
}

int main(int argc, char **argv)
{
    try {
        syscat.open();
    }
    catch (runtime_error err) {
        cerr << "Error: " << err.what() << endl;
        return EXIT_FAILURE;
    }
    
    char c;
    comboTest();
    return 0;
}
