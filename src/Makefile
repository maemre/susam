# Edit these according to your platform. I tested it with Clang
# compiler and suggest you to use it, but any recent version of
# GCC will compile probably, I suggest you to use GCC 4.8.x

# change this to g++ if you're using GCC
CPP=clang++
CPPFLAGS=-std=c++11 -stdlib=libc++ -Wall -g
# change this to ld or g++ if you're using GCC
LINKER=clang++
LFLAGS=-lc++
TARGET=main

# some people and platforms use `make all', so let's use it
all: $(TARGET)

$(TARGET): main.o cli.o util.o table.o record.o syscat.o operations.o metadata.o datapage.o
	$(LINKER) $(LFLAGS) main.o cli.o util.o table.o record.o syscat.o operations.o metadata.o datapage.o -o $(TARGET)
	cp $(TARGET) ../bin

metadata.o: metadata.cpp metadata.h util.h table.h
	$(CPP) -c $(CPPFLAGS) metadata.cpp

datapage.o: datapage.cpp datapage.h util.h table.h record.h
	$(CPP) -c $(CPPFLAGS) datapage.cpp

util.o: util.cpp util.h
	$(CPP) -c $(CPPFLAGS) util.cpp

cli.o: cli.cpp cli.h util.h operations.h
	$(CPP) -c $(CPPFLAGS) cli.cpp

main.o: main.cpp util.h operations.h cli.h
	$(CPP) -c $(CPPFLAGS) main.cpp

table.o: table.cpp util.h table.h
	$(CPP) -c $(CPPFLAGS) table.cpp

record.o: record.cpp record.h table.h util.h
	$(CPP) -c $(CPPFLAGS) record.cpp

syscat.o: syscat.h syscat.cpp table.h util.h
	$(CPP) -c $(CPPFLAGS) syscat.cpp

operations.o: operations.h operations.cpp syscat.h datapage.h metadata.h record.h util.h
	$(CPP) -c $(CPPFLAGS) operations.cpp

tests: test-dir syscat_test
	./test-dir/syscat_test

test-dir:
	mkdir -p test-dir

syscat_test: syscat_test.o table.o syscat.o util.o
	$(LINKER) $(LFLAGS) syscat_test.o table.o syscat.o util.o -o syscat_test
	mv metadata_test test-dir/

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
