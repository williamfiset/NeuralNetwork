
# Compiler
CC = g++

# The options being passed to compiler
CFLAGS = -g -Wall -Wno-unused -O3 -std=c++0x

object_files = neuralnetworkmain.o neuralnetwork.o helpers.o
EXECUTABLES = neuralNetMain

all: $(EXECUTABLES)

neuralNetMain: $(object_files)
	$(CC) $(object_files) $(CFLAGS) -lm -o neuralNetMain

# Object file compilations 

neuralnetworkmain.o: neuralnetworkmain.cpp
	$(CC) $(CFLAGS) -c neuralnetworkmain.cpp

neuralnetwork.o: neuralnetwork.cpp neuralnetwork.hpp
	$(CC) $(CFLAGS) -c neuralnetwork.cpp

helpers.o: helpers.cpp helpers.hpp
	$(CC) $(CFLAGS) -c helpers.cpp

clean:
	rm -f $(object_files)
	rm -f $(EXECUTABLES)

