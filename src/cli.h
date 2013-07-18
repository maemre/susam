#ifndef _CLI_H
#define _CLI_H

#include <iosfwd>
#include <unordered_map>
#include <string>

void menu();
void CLI_init();
void repl();

typedef void (*callback_t)(const std::string &);

struct command_t {
    std::string name;
    std::string usage;
    bool (*argc_check)(int);
    callback_t callback;
    // constructors:
    // the empty constructor
    command_t(std::string name_ = "");
    // use this constructor by default
    command_t(std::string name_, std::string usage_, bool (*argc_c)(int), callback_t cb);
};

#endif
