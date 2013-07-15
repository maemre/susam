#ifndef _CLI_H
#define _CLI_H

#include <iosfwd>
#include <unordered_map>
#include <string>

void menu();
void create_commands();
void repl();

struct command_t {
    std::string name;
    std::string usage;
    bool (*argc_check)(int);
    void (*callback)(void);
    // constructors:
    // the empty constructor
    command_t(std::string name_="");
    // use this constructor by default
    command_t(std::string name_, std::string usage_, bool (*argc_c)(int), void (*cb)(void));
};

#endif
