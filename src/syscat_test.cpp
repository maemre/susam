#include <iostream>
#include "syscat.h"
#include <stdexcept>

using namespace std;

SystemCatalogue syscat;

void createTableTest()
{
    cout << "Creating two tables" << endl;
    Table t;
    t.isDeleted=false;
    // Table 1
    t.name="students";
    t.pk_index=0;
    t.fields.push_back(make_pair("student id", 10));
    t.fields.push_back(make_pair("name", 15));
    t.fields.push_back(make_pair("dept", 4));
    t.fields.push_back(make_pair("email", 100));
    cout << "Result: " << syscat.createTable(t) << endl;

    t.name="courses";
    t.pk_index=0;
    t.fields.clear();
    t.fields.push_back(make_pair("course id", 4));
    t.fields.push_back(make_pair("dept", 4));
    t.fields.push_back(make_pair("year", 4));
    cout << "Result: " << syscat.createTable(t) << endl;
}

void listTablesTest()
{
    cout << "Listing all tables." << endl;
    syscat.listAllTables();
}

int main(int argc, char **argv)
{   
    try {
        syscat.open();
    } catch (runtime_error err) {
        cerr << "Error: " << err.what() << endl;
        return EXIT_FAILURE;
    }
    
    createTableTest();
    listTablesTest();

    return 0;
}
