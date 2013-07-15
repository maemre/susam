# Edit these according to your platform. I tested it with Clang
# compiler and suggest you to use it, but any recent version of
# GCC will compile probably, I suggest you to use GCC 4.8.x

# change this to g++ if you're using GCC
CC=clang++
CXXFLAGS=-std=c++11 -stdlib=libc++ -Wall -g
# change this to ld or g++ if you're using GCC
LINKER=clang++
LFLAGS=-lc++

all: main

main: main.o cli.o util.o
	$(LINKER) $(LFLAGS) main.o cli.o util.o -o main

util.o: util.cpp util.h
	$(CC) -c $(CXXFLAGS) util.cpp

cli.o: cli.cpp cli.h util.h
	$(CC) -c $(CXXFLAGS) cli.cpp

main.o: main.cpp cli.h
	$(CC) -c $(CXXFLAGS) main.cpp

clean:
	rm -fr *.o *.dSYM

distclean: clean
	rm -f main

run: all
	./main
