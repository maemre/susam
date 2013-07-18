#include "operations.h"
#include "syscat.h"

SystemCatalogue syscat;

void init_managers()
{
    syscat.open();
}

void listAllTables()
{
    syscat.listAllTables();
}
