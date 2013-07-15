#include <iostream>
#include <string>
#include "cli.h"

using namespace std;

int main(int argc, char **argv) {
    CLI_init();

    cout << "Welcome to Simple Storage Manager" << endl
        << "Type 'help' to get a full list of commands." << endl;
    
    // Oh! Minimalism, my favourite
    while (cin)
        repl(); // magic!
    return 0;
}
