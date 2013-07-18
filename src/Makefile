# Edit these according to your platform. I tested it with Clang
# compiler and suggest you to use it, but any recent version of
# GCC will compile probably, I suggest you to use GCC 4.8.x

# change this to g++ if you're using GCC
CC=clang++
CXXFLAGS=-std=c++11 -stdlib=libc++ -Wall -g
# change this to ld or g++ if you're using GCC
LINKER=clang++
LFLAGS=-lc++
TARGET=main

$(TARGET): main.o cli.o util.o table.o record.o syscat.o operations.o
	$(LINKER) $(LFLAGS) main.o cli.o util.o table.o record.o syscat.o operations.o -o $(TARGET)
	cp main ../bin

util.o: util.cpp util.h
	$(CC) -c $(CXXFLAGS) util.cpp

cli.o: cli.cpp cli.h util.h operations.h
	$(CC) -c $(CXXFLAGS) cli.cpp

main.o: main.cpp cli.h
	$(CC) -c $(CXXFLAGS) main.cpp

table.o: table.cpp table.h
	$(CC) -c $(CXXFLAGS) table.cpp

record.o: record.cpp record.h table.h
	$(CC) -c $(CXXFLAGS) record.cpp

syscat.o: syscat.h syscat.cpp table.h
	$(CC) -c $(CXXFLAGS) syscat.cpp

operations.o: operations.h operations.cpp syscat.h
	$(CC) -c $(CXXFLAGS) operations.cpp



clean:
	rm -fr *.o *.dSYM

distclean: clean
	rm -f $(TARGET)

cleanstyle:
	rm -f *.orig

style:
	astyle --options=astylerc *.cpp *.h 

run: all
	cd ../bin
	./main