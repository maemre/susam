#include <iostream>
#include <stdexcept>
#include "syscat.h"
#include "util.h"
#include <sstream>
#include <cstdlib>
#include <cstdio>

using namespace std;

SystemCatalogue::SystemCatalogue()
    : linesRead_(), pos_(0)
{
}

void SystemCatalogue::open()
{
    fs.open("syscat.txt", fstream::in | fstream::out | fstream::binary);
    
    if (!fs.is_open()) { // file may not exists, so open in out-only mode to create the file
        // open in write-only mode to create the file
        fs.open("syscat.txt", fstream::out);
        fs.close(); // close the file
        // try opening file again
        fs.open("syscat.txt", fstream::in | fstream::out | fstream::binary);
    }
    
    if (! fs.is_open())
        throw runtime_error("Cannot open system catalogue file");
}

SystemCatalogue::~SystemCatalogue()
{
    if (fs.is_open())
        fs.close();
}

Table parseLine(string s)
{
    stringstream line(s);
    char deleted;
    int nOfFields;
    Table t;
    line >> deleted;
    if (! line) // If stream is not readable, we're done
        return t;
    if (deleted == '1') {
        t.isDeleted = true;
        return t;
    }
    
    line >> t.name; // Table name does not contain spaces, thanks to Goddess
    line >> nOfFields;
    line >> t.pk_index;

    // Read fields:
    for (int i = 0; i < nOfFields; i++) {
        pair<string, int> field;
        string temp;
        // read till next colon (field name)
        getline(line, field.first, ':');
        triml(field.first);
        // read till next comma (field size)
        getline(line, temp, ',');
        field.second = atoi(temp.c_str());

        t.fields.push_back(field);

        if (! line) // If stream is not readable, we're done
            return t;
    }
    t.isDeleted = false;
    return t;
}

void SystemCatalogue::listAllTables()
{
    string line;
    
    // Go to the beginning of the file
    fs.seekg(0, ios::beg);
    
    while (getline(fs, line)) {
        Table t = parseLine(line);
        if (t.isDeleted)
            continue;
        cout << t << endl;
    }
    
    // Go back to where we left off
    fs.seekg(pos_, ios::beg);
}

Table SystemCatalogue::findTable(const string &name)
{
    Table t;
    
    // Try memory cache, if fails use disk
    for (auto & line : linesRead_) {
        t = parseLine(line);
        
        if (t.name == name && !t.isDeleted)
            return t;
    }
    
    // Do disk IO:
    string line;
    
    while (getline(fs, line)) {
        linesRead_.push_back(line);
        t = parseLine(line);
        pos_ += line.size() + 1; // Extra byte for EOL character
        
        if (t.name == name && !t.isDeleted)
            return t;
    }
    fs.clear();
    return Table();
}

bool SystemCatalogue::deleteTable(const string &name)
{
    int p = 0;
    Table t;
    int i = 0;
    // Use memory cache
    for (auto & line : linesRead_) {
        t = parseLine(line);
        p += line.size() + 1;
        
        if (t.name == name && !t.isDeleted) {
            int pos = fs.tellp();
            fs.seekp(p, ios::beg);
            fs << '1'; // Set IsDeleted flag to 1 for that table
            linesRead_[i][0]='1'; // Set that flag also in memory
            fs.sync();
            fs.seekp(pos, ios::beg); // go back to where you were
            return true; // success
        }
        ++i;
    }
    
    // Do disk IO
    string line;
    
    while (getline(fs, line)) {
        t = parseLine(line);
        pos_ += line.size() + 1;
        
        if (t.name == name && !t.isDeleted) {
            fs.seekp(pos_ - line.size() + 1, ios::beg);
            fs << '1'; // Set IsDeleted flag to 1 for that table
            fs.sync();
            fs.seekp(pos_, ios::beg); // go back to where you were
            line[0] = '1'; // Set IsDeleted for that line
            linesRead_.push_back(line);
            return true; // success
        }
        
        linesRead_.push_back(line);
    }
    
    // Table not found:
    return false;
}

bool SystemCatalogue::createTable(const Table &t)
{
    stringstream ss;
    string line;
    Table u=findTable(t.name);
    if (u.name == t.name) // If there is already a table
        return false;
    fs.seekp(0, ios::end);
    if (t.isDeleted) // A deleted table counts as added
        return true;
    char deleted = t.isDeleted ? '1' : '0';
    ss << deleted
        << t.name << ' '
        << t.fields.size() << ' '
        << t.pk_index << ' '; // fix this
    
    // Write field names and sizes, they're comma seperated values
    for (auto & field : t.fields)
        ss << field.first << ':' << field.second << ',';
    ss << endl;
    line = ss.str();
    fs << line;
    fs.flush(); // Guarantee that we've written everything.
    fs.seekg(pos_, ios::beg);
    fs.seekp(pos_, ios::beg);
    return true;
}
