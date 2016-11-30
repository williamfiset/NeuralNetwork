# William Fiset

# Compiler
CC = g++

# CFLAGS are the options being passed to compiler
CFLAGS = -g -Wall -Wno-unused -O3 -std=c++0x

object_files = neuralnetworkmain.o neuralnetwork.o helpers.o
test_files = T.o helpers.o neuralnetwork.o
EXECUTABLES = neuralNetMain TEST

all: $(EXECUTABLES)

neuralNetMain: $(object_files)
	$(CC) $(object_files) $(CFLAGS) -lm -o neuralNetMain

TEST: $(test_files)
	$(CC) $(test_files) $(CFLAGS) -lm -o TEST

# Object file compilations 
Main.o: Main.cpp  # Add other dependencies here. such as Main.hpp 
	$(CC) $(CFLAGS) -c Main.cpp

neuralnetworkmain.o: neuralnetworkmain.cpp
	$(CC) $(CFLAGS) -c neuralnetworkmain.cpp

neuralnetwork.o: neuralnetwork.cpp neuralnetwork.hpp
	$(CC) $(CFLAGS) -c neuralnetwork.cpp

helpers.o: helpers.cpp helpers.hpp
	$(CC) $(CFLAGS) -c helpers.cpp

T.o: T.cpp
	$(CC) $(CFLAGS) -c T.cpp

clean:
	rm -f $(object_files)
	rm -f $(test_files)
	rm -f $(EXECUTABLES)








