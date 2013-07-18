#include "cli.h"
#include "util.h"
#include "operations.h"
#include <iostream>

using namespace std;

command_t::command_t(string name_)
    : name(name_), usage(""), argc_check(0), callback(0)
{
}

command_t::command_t(string name_, string usage_, bool (*argc_c)(int), callback_t cb)
    : name(name_), usage(usage_), argc_check(argc_c), callback(cb)
{
}

std::unordered_map<std::string, command_t> commands;

void CLI_init()
{
    command_t cmd;
    
    // Table operations
    commands["create"] = command_t(
                             "create",
                             "usage: create [table-name] [index-of-primary-key] [number-of-fields] [field-name:field-size,]+",
    [](int argc) {
        return argc >= 4;
    },
    [](const string &args) {
        
    }
                         );
    commands["delete-table"] = command_t(
                                   "delete-table",
                                   "usage: delete-table [table-name]",
    [](int argc) {
        return argc == 2;
    },
    [](const string &args) {
        cout << "delete table, to be implemented\n";
    }
                               );
    commands["list-tables"] = command_t(
                                  "list-tables",
                                  "usage: list-tables",
    [](int argc) {
        return argc == 1;
    },
    [](const string &args) {
        listAllTables();
    }
                              );
                              
    // Record operations
    commands["insert"] = command_t(
                             "insert",
                             "usage: insert [table-name] [field-value,]",
    [](int argc) {
        return argc >= 3;
    },
    [](const string &args) {
        cout << "insert record, to be implemented";
    }
                         );
    commands["delete"] = command_t(
                             "delete",
                             "usage: delete [table-name] [primary-key]",
    [](int argc) {
        return argc == 3;
    }, // argc==3 since primary key cannot contain spaces
    [](const string &args) {
        cout << "delete record, to be implemented";
    }
                         );
    commands["update"] = command_t(
                             "update",
                             "usage: update [table-name] [field-value,]",
    [](int argc) {
        return argc >= 3;
    },
    [](const string &args) {
        cout << "update record, to be implemented";
    }
                         );
    commands["list"] = command_t(
                           "list",
                           "usage: list [table-name]",
    [](int argc) {
        return argc == 2;
    },
    [](const string &args) {
        cout << "list all records in a table, to be implemented";
    }
                       );
    commands["search"] = command_t(
                             "search",
                             "usage: search [table-name] [operation] [primary-key-value]",
    [](int argc) {
        return argc == 4; // primary key value cannot contain spaces
    },
    [](const string &args) {
        cout << "search records, to be implemented";
    }
                         );
                         
    //Miscellaneous
    commands["help"] = command_t(
                           "help",
                           "usage: help",
    [](int argc) {
        return argc == 1;
    },
    [](const string &args) {
        menu();
    }
                       );
    commands["quit"] = commands["q"] = command_t(
                                           "quit",
                                           "usage: quit",
    [](int argc) {
        return argc == 1;
    },
    [](const string &args) {
        exit(EXIT_SUCCESS);
    }
                                       );
                                       
    cout << "Welcome to Simple Storage Manager" << endl
         << "Type 'help' to get a full list of commands." << endl;
}

void repl()
{
    string input_line;
    cout << "> ";
    getline(cin, input_line);
    
    auto v = split(input_line, ' ');
    
    if (v.empty())
        return;
        
    for (auto & cmd : commands) {
        if (v[0] == cmd.first) {
            if (cmd.second.argc_check(v.size())) {
                // Find first space in input line
                int space;
                for (space=0; space<input_line.size(); ++space) {
                    if (input_line[space]==' ') {
                        ++space; // set i to the character after space
                        break;
                    }
                }
                string args = input_line.substr(space);
                cmd.second.callback(input_line);
            }
            else {
                cout << cmd.second.usage << endl;
            }
            
            return;
        }
    }
    
    cout << "Error: Unrecognised command: '" << v[0]
         << "'. For command list, type help" << endl;
}

void menu()
{
    cout << "The commands available are:" << endl
         << endl
         << "Table Operations:" << endl
         << "\tcreate [table-name] [index-of-primary-key] [number-of-fields] [field-name:field-size]+ -- create a table" << endl
         << "\tdelete-table [table-name] -- delete a table" << endl
         << "\tlist-tables -- list all tables" << endl
         << endl
         << "Record Operations: " << endl
         << "\tinsert [table-name] [field-value,]+ -- insert a record to a table" << endl
         << "\tdelete [table-name] [primary-key] -- delete the record with primary-key from table" << endl
         << "\tupdate  [table-name] [field-value,]+ -- update value of a record in a table" << endl
         << "\tlist [table-name] -- list all records in the table" << endl
         << "\tsearch [table-name] [operator] [primary-key] -- search according to primary key" << endl
         << "\thelp -- show this message" << endl
         << "\tquit or q -- quit the program" << endl
         << endl
         << "For more information on specifics of usage, please consult to README file" << endl
         << endl;
}
