# Makefile for bin/test
# created with makefile-creator


####################
#Settings:


CC = g++
CFLAGS =  -Wall -Wextra -pedantic -O3 -std=c++11 -DDEBUG 
CLIBS = 
INCLUDES = 
TARGET = bin/test
OBJECTS = build/output.o build/test.o


####################
#Rules:


$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(CLIBS)

build/%.o:
	@if test ! -d 'build'; then mkdir 'build'; echo "created 'build'" ; fi
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm build/*.o

all: $(TARGET)


####################
#Dependencies:


build/output.o: src/output.cpp src/output.hpp src/output.tcc 

build/test.o: src/test.cpp src/output.hpp src/output.tcc 

