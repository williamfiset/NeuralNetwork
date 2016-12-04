# NeuralNetwork

### How to run neural network

Make sure you have a g++ compiler that supports at minimum c++0x
then you should be able to compile the files for the Neural Network

``` bash
$ make
g++ -g -Wall -Wno-unused -O3 -std=c++0x -c neuralnetworkmain.cpp
g++ -g -Wall -Wno-unused -O3 -std=c++0x -c neuralnetwork.cpp
g++ -g -Wall -Wno-unused -O3 -std=c++0x -c helpers.cpp
g++ neuralnetworkmain.o neuralnetwork.o helpers.o -g -Wall -Wno-unused -O3 -std=c++0x -lm -o neuralNetMain
```

### Running the network

You have the choice between running the network on one individual training and test set or running the network on multiple training and test sets. For the examples below the training and test sets have already been generated with the generate_jackknife_sets.py file and placed in the data/ folder.

To run an individual training and test set use the -s flag (s stands for single)
``` bash
./neuralNetMain -s data/iris/iris_data.4.train data/iris/iris_data.4.test
```

To run multiple training and test sets use the -d flag (d for directory) and specify a directory containing *.train and *.test file pairs
``` bash
./neuralNetMain -d data/iris/
```

### Creating your own datasets



