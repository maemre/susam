#include <iostream>
#include <string>
#include "operations.h"
#include "cli.h"

using namespace std;

int main(int argc, char **argv)
{
    init_managers();
    CLI_init();
    
    // Oh! Minimalism, my favourite
    while (cin)
        repl(); // magic!
        
    return 0;
}
