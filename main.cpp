#include <iostream>
#include <string>
#include "cli.h"

using namespace std;

int main(int argc, char **argv) {
    CLI_init();

    // Oh! Minimalism, my favourite
    while (cin)
        repl(); // magic!
    return 0;
}
