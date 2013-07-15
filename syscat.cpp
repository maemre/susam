#include <iostream>
#include <stdexcept>
#include "syscat.h"
#include <sstream>

using namespace std;

SystemCatalogue::SystemCatalogue()
    : pos_(0), fs("syscat.txt"), linesRead_()
{
    if (! fs.is_open())
        throw runtime_error("Cannot open system catalogue file");
}

SystemCatalogue::~SystemCatalogue()
{
    fs.close();
}

Table parseLine(string s) {
    stringstream line(s);
    char deleted;
    int nOfFields;
    Table t;
    line >> deleted;
    if (deleted == '0') {
        t.isDeleted = true;
        return t;
    }
    line >> t.name; // Table name does not contain spaces, thanks to Goddess
    line >> nOfFields;
    line >> t.pk_index;
    // Read field names:
    for (int i = 0; i < nOfFields; i++) {
        pair<string, int> field;
        line >> field.first;
        t.fields.push_back(field);
    }
    // Read field sizes
    for (auto &field : t.fields)
        line >> field.second;
    
    return t;
}

void SystemCatalogue::listAllTables()
{
    string line;

    // Go to the beginning of the file
    fs.seekg(0, fs.beg);
    while (getline(fs, line)) {
        Table t=parseLine(line);
        cout << t << endl;
    }

    // Go back to where we left off
    fs.seekg(pos_, fs.beg);
}

Table SystemCatalogue::findTable(const string &name)
{
    Table t;
    // Try memory cache, if fails use disk
    for (auto &line : linesRead_) {
        t=parseLine(line);
        if (t.name == name && !t.isDeleted)
            return t;
    }
    // Do disk IO:
    string line;
    while (getline(fs, line)) {
        linesRead_.push_back(line);
        t=parseLine(line);
        pos_+=line.size()+1; // Extra byte for EOL character
        if (t.name == name && !t.isDeleted)
            return t;
    }
    return Table();
}

bool SystemCatalogue::deleteTable(const string& name)
{
    int p=0;
    Table t;
    // Use memory cache
    for (auto &line : linesRead_) {
        t=parseLine(line);
        p+=line.size()+1;
        if (t.name == name && !t.isDeleted) {
            int pos = fs.tellp();
            fs.seekp(p);
            fs << '1'; // Set IsDeleted flag to 1 for that table
            fs.sync();
            fs.seekp(pos); // go back to where you were
            return true; // success
        }
    }
    // Do disk IO
    string line;
    while (getline(fs, line)) {
        t=parseLine(line);
        pos_+=line.size()+1;
        if (t.name == name && !t.isDeleted) {
            fs.seekp(pos_-line.size()+1, fs.beg);
            fs << '1'; // Set IsDeleted flag to 1 for that table
            fs.sync();
            fs.seekp(pos_); // go back to where you were
            line[0]='1'; // Set IsDeleted for that line
            linesRead_.push_back(line);
            return true; // success
        }
        linesRead_.push_back(line);
    }
    // Table not found:
    return false;
}


