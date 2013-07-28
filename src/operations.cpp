#include "operations.h"
#include "syscat.h"
#include <string>
#include <iostream>
#include <cctype>
#include <cstdio>
#include "metadata.h"
#include "datapage.h"
#include <array>

using namespace std;

SystemCatalogue syscat;
fstream dataFile;

void init_managers()
{
    syscat.open();
    
    dataFile.open("data.bin", ios::in | ios::out | ios::binary);
    
    if (!dataFile) {
        dataFile.open("data.bin", ios::out | ios::binary);
        dataFile.close(); // create data file
        dataFile.open("data.bin", ios::in | ios::out | ios::binary);
    }
}

void deleteTable(const string &name)
{
    if (name.empty()) {
        cout << "Table name cannot be empty." << endl;
        return;
    }
    
    for (auto c : name) {
        if (c != '\0' && c != '-' && c != '_' && !isalnum(c)) {
            cout << "Table name must contain alphanumeric characters or '-' or '_' only." << endl;
            return;
        }
    }
    
    if (syscat.deleteTable(name)) {
        if (remove((name + ".dat").c_str()) != 0)
            cout << "Table meta data file could not be deleted." << endl;
        else
            cout << "Table '" << name << "' deleted successfully." << endl;
    }
    else
        cout << "Table '" << name << "' could not be deleted since it does not exist." << endl;
}

void createTable(const Table &t)
{
    if (syscat.createTable(t)) {
        // Create table metadata file
        fstream fs(t.name + ".dat", ios::out);
        fs.close();
        cout << "Table '" << t.name << "' created successfully." << endl;
    }
    else
        cout << "Table '" << t.name << "' could not be created since it already exists." << endl;
}

void listAllTables()
{
    syscat.listAllTables();
}

bool check_values(const Table &t, const vector<string> v)
{
    if (t.fields.size() != v.size()) {
        cout << "Incorrect number of values" << endl;
        return false;
    }
    
    for (char c : v[t.pk_index]) {
        if (!isdigit(c)) {
            cout << "Primary key must be an integer" << endl;
            return false;
        }
    }
    
    return true;
}

void read_page(int pageId, array<char, PAGE_SIZE> &page)
{
    dataFile.clear();
    dataFile.seekg(pageId * PAGE_SIZE);
    dataFile.read(page.data(), PAGE_SIZE);
    cout << "Reading page #" << pageId << " of data file." << endl;
}

void write_page(int pageId, const array<char, PAGE_SIZE> &page)
{
    dataFile.clear();
    dataFile.seekp(pageId * PAGE_SIZE);
    dataFile.write(page.data(), PAGE_SIZE);
    dataFile.flush();
    dataFile.sync();
    cout << "Writing to page #" << pageId << " of data file." << endl;
}

int create_new_page(Metadata &m)
{
    dataFile.seekp(0, ios::end);
    int vacantPageId = (int) dataFile.tellp() / PAGE_SIZE;
    m.bindNewPage(vacantPageId);
    return vacantPageId;
}

void insertRecord(const string &name, const vector<string> values)
{
    Table table = syscat.findTable(name);
    
    if (table.isDeleted) {
        cout << "Table '" << name << "' not found." << endl;
        return;
    }
    
    if (!check_values(table, values))
        return;
        
    dataFile.clear();
    
    try {
        Record r(table, values);
        Metadata m(table);
        array<char, PAGE_SIZE> page;
        int vacantPageId = -1;
        int pageId;
        
        while ((pageId = m.nextPageId()) > -1) {
            read_page(pageId, page);
            DataPage d(page, table);
            
            if (d.state == State::vacant)
                vacantPageId = pageId;
                
            string pk_field = table.fname(table.pk_index);
            
            for (auto & r : d.records) {
                if (r.values[pk_field] != values[table.pk_index])
                    continue;
                    
                cout << "Error: A record with primary key value '"
                     << values[table.pk_index]
                     << "' already exists." << endl;
                return;
            }
            
            dataFile.clear();
        }
        
        if (vacantPageId == -1) { // Add a new page
            vacantPageId = create_new_page(m);
            page.fill(0); // clear page
        }
        else {
            dataFile.seekg(vacantPageId * PAGE_SIZE);
            dataFile.read(page.data(), PAGE_SIZE);
        }
        
        // Write it baby
        DataPage d(page, table);
        d.addRecord(r);
        d.write(page);
        write_page(vacantPageId, page);
        cout << "Record " << r << " inserted successfully to table '" << name << "'." << endl;
    }
    catch (bad_alloc e) {
        cout << "Error: out of memory. Message: " << e.what() << endl;
        return;
    }
    catch (runtime_error e) {
        cout << "Error: " << e.what() << endl;
        return;
    }
}

void updateRecord(const string &name, const vector<string> values)
{
    Table table = syscat.findTable(name);
    
    if (table.isDeleted) {
        cout << "Table '" << name << "' not found." << endl;
        return;
    }
    
    if (!check_values(table, values))
        return;
        
    dataFile.clear();
    
    try {
        Record r(table, values);
        Metadata m(table);
        array<char, PAGE_SIZE> page;
        int pageId;
        
        while ((pageId = m.nextPageId()) > -1) {
            read_page(pageId, page);
            DataPage d(page, table);
            
            string pk_field = table.fname(table.pk_index);
            
            for (auto & r : d.records) {
                if (r.values[pk_field] == values[table.pk_index]) {
                    for (int i = 0; i < table.fields.size(); ++i)
                        r.values[table.fname(i)] = values[i];
                        
                    d.write(page);
                    write_page(pageId, page);
                    cout << "Record " << r << " updated successfully." << endl;
                    return;
                }
            }
        }
        
        
        cout << "Error: A record with primary key value '"
             << values[table.pk_index]
             << "' does not exist." << endl;
    }
    catch (bad_alloc e) {
        cout << "Error: out of memory. Message: " << e.what() << endl;
        return;
    }
    catch (runtime_error e) {
        cout << "Error: " << e.what() << endl;
        return;
    }
}

void deleteRecord(const string &name, const string &pk_value)
{
    Table table = syscat.findTable(name);
    
    if (table.isDeleted) {
        cout << "Table '" << name << "' not found." << endl;
        return;
    }
    
    for (char c : pk_value) {
        if (!isdigit(c) && c != '\0') {
            cout << "Primary key must be an integer!" << endl;
            return;
        }
    }
    
    dataFile.clear();
    
    try {
        Metadata m(table);
        array<char, PAGE_SIZE> page;
        int pageId;
        
        while ((pageId = m.nextPageId()) > -1) {
            read_page(pageId, page);
            DataPage d(page, table);
            
            string pk_field = table.fname(table.pk_index);
            int i = 0;
            bool found = false;
            
            for (auto & r : d.records) {
                if (r.values[pk_field] == pk_value) {
                    found = true;
                    break;
                }
                
                ++i;
            }
            
            if (found) {
                auto it = d.records.begin() + i;
                d.records.erase(it);
                d.write(page);
                write_page(pageId, page);
                cout << "Record with PK value '" << pk_value << "' deleted successfully." << endl;
                return;
            }
        }
        
        
        cout << "Error: A record with primary key value '"
             << pk_value
             << "' does not exist." << endl;
    }
    catch (bad_alloc e) {
        cout << "Error: out of memory. Message: " << e.what() << endl;
        return;
    }
    catch (runtime_error e) {
        cout << "Error: " << e.what() << endl;
        return;
    }
}

void searchRecords(const string &name, comparator c, const std::string &pk_value)
{
    Table table = syscat.findTable(name);
    
    if (table.isDeleted) {
        cout << "Table '" << name << "' not found." << endl;
        return;
    }
    
    for (char c : pk_value) {
        if (!isdigit(c) && c != '\0') {
            cout << "Primary key must be an integer!" << endl;
            return;
        }
    }
    
    dataFile.clear();
    
    try {
        Metadata m(table);
        array<char, PAGE_SIZE> page;
        int pageId;
        
        string pk_field = table.fname(table.pk_index);
        int pk_val = atoi(pk_value.c_str());
        
        while ((pageId = m.nextPageId()) > -1) {
            read_page(pageId, page);
            DataPage d(page, table);
            
            for (auto & r : d.records)
                if (c(atoi(r.values[pk_field].c_str()), pk_val))
                    cout << r << endl;
        }
    }
    catch (bad_alloc e) {
        cout << "Error: out of memory. Message: " << e.what() << endl;
        return;
    }
    catch (runtime_error e) {
        cout << "Error: " << e.what() << endl;
        return;
    }
}

void listAllRecords(const string &name)
{
    Table table = syscat.findTable(name);
    
    if (table.isDeleted) {
        cout << "Table '" << name << "' not found." << endl;
        return;
    }
    
    dataFile.clear();
    
    try {
        Metadata m(table);
        array<char, PAGE_SIZE> page;
        int pageId;
        
        while ((pageId = m.nextPageId()) > -1) {
            read_page(pageId, page);
            DataPage d(page, table);
            
            for (auto & r : d.records)
                cout << r << endl;
        }
    }
    catch (bad_alloc e) {
        cout << "Error: out of memory. Message: " << e.what() << endl;
        return;
    }
    catch (runtime_error e) {
        cout << "Error: " << e.what() << endl;
        return;
    }
}
