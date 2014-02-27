A Storage Manager
=================

- Language used: C++11
- Compiler used: Clang with C++11 extensions and libc++ as standard library

Project for Cmpe 321 course. A simple storage manager that can handle simple table operations with a page by page IO.

Build Process
-------------

I've written the project in C++11, so you need a compiler that's compliant with C++11. GCC 4.8 and Clang 3.4 can be used for this manner. I perdonally preferred Clang over GCC. If you want to compile the project with GCC, change the line `CPP=clang++` to `CPP=g++` in Makefile. The Makefile can be found in src/ directory along with the code itself.

### Compiling the code

First go into `src/` directory in a terminal, then use the following command to build the program:

    make

After that there will be an executable file named main in `bin/` directory.

### Running the program

To run the program you can use `make run` command in src directory if you don't want to change directories. Also you can go into `bin/` directory and simply run the command `./main`.

Using The Program
-----------------

When you run the program in a terminal, you will see a command prompt, to check out the commands' synopses and functions, you can simply enter the command `help`.

The commands defined in program are:
+ Table Operations:
 + `create [table-name] [index-of-primary-key] [field-name:field-size]+`: Create a table
 + `delete-table [table-name]`: Delete a table
 + `list-tables`: List all tables
+ Record Operations: 
 + `insert [table-name] [field-value,]+`: Insert a record to a table
 + `delete [table-name] [primary-key]`: Delete the record with primary-key from table
 + `update  [table-name] [field-value,]+`: Update value of a record in a table
 + `list [table-name]`: List all records in the table
 + `search [table-name] [operator] [primary-key]`: Search according to primary key
 + `help`: Show this message
 + `quit` or `q`: Quit the program

