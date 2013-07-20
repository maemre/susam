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

void insertRecord(const string &name, const vector<string> values)
{
    Table table = syscat.findTable(name);
    
    if (table.isDeleted) {
        cout << "Table '" << name << "' not found." << endl;
        return;
    }
    
    if (table.fields.size() != values.size()) {
        cout << "Incorrect number of values" << endl;
        return;
    }
    
    for (char c : values[table.pk_index]) {
        if (!isdigit(c)) {
            cout << "Primary key must be an integer" << endl;
            return;
        }
    }
    
    Record r(table, values);
    dataFile.clear();
    
    try {
        Metadata m(table);
        
        array<char, PAGE_SIZE> page;
        
        int vacantPageId = -1;
        int pageId;
        
        while ((pageId = m.nextPageId()) > -1) {
            dataFile.clear();
            dataFile.seekg(pageId * PAGE_SIZE);
            dataFile.read(page.data(), PAGE_SIZE);
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
            dataFile.seekp(0, ios::end);
            vacantPageId = (int) dataFile.tellp() / PAGE_SIZE;
            m.bindNewPage(vacantPageId);
            page.fill(0); // clear page
        }
        else {
            dataFile.seekg(vacantPageId * PAGE_SIZE);
            dataFile.read(page.data(), PAGE_SIZE);
        }
        
        // Write it baby
        dataFile.seekp(vacantPageId);
        dataFile.clear();
        DataPage d(page, table);
        d.addRecord(r);
        d.write(page);
        // for (int i=0; i<PAGE_SIZE; ++i)
        //     printf("%02x ", (int)page[i]);
        // cout << endl;
        dataFile.write(page.data(), PAGE_SIZE);
        dataFile.flush();
        dataFile.sync();
    }
    catch (bad_alloc e) {
        cout << "Error: out of memory. Message: " << e.what() << endl;
        return;
    }
    catch (runtime_error e) {
        cout << "Error: " << e.what();
        return;
    }
    
    cout << "Record " << r << " inserted successfully to table '" << name << "'." << endl;
}
