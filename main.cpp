#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include "cli.h"

using namespace std;

int main(int argc, char **argv) {
  
    // Make cin not sync with stdio, which allows buffered
    // read but f*cks up scanf
    // cin.sync_with_stdio(false);
    
    create_commands();

    cout << "Welcome to Simple Storage Manager" << endl
        << "Type 'help' to get a full list of commands." << endl;
    while (cin) repl();
    return 0;
}
