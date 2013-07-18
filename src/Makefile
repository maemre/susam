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

# some people and platforms use `make all', so let's use it
all: $(TARGET)

$(TARGET): main.o cli.o util.o table.o record.o syscat.o operations.o
	$(LINKER) $(LFLAGS) main.o cli.o util.o table.o record.o syscat.o operations.o -o $(TARGET)
	cp $(TARGET) ../bin

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

syscat.o: syscat.h syscat.cpp table.h util.h
	$(CC) -c $(CXXFLAGS) syscat.cpp

operations.o: operations.h operations.cpp syscat.h
	$(CC) -c $(CXXFLAGS) operations.cpp

tests: test-dir syscat_test
	./test-dir/syscat_test

test-dir:
	mkdir -p test-dir

syscat_test: syscat_test.o table.o syscat.o util.o
	$(LINKER) $(LFLAGS) syscat_test.o table.o syscat.o util.o -o syscat_test
	mv syscat_test test-dir/

clean:
	rm -fr *.o *.dSYM

distclean: clean
	rm -fr test-dir
	rm -f $(TARGET) ../bin/$(TARGET)

cleanthemall: distclean
	rm -f ../bin/*

cleanstyle:
	rm -f *.orig

style:
	astyle --options=astylerc *.cpp *.h 

run: all
	cd ../bin ; ./$(TARGET)
