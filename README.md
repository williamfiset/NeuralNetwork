# NeuralNetwork

## How to compile the neural network

Make sure you have a g++ compiler that supports at minimum c++0x
then you should be able to compile the files for the Neural Network

```
$ make
g++ -g -Wall -Wno-unused -O3 -std=c++0x -c neuralnetworkmain.cpp
g++ -g -Wall -Wno-unused -O3 -std=c++0x -c neuralnetwork.cpp
g++ -g -Wall -Wno-unused -O3 -std=c++0x -c helpers.cpp
g++ neuralnetworkmain.o neuralnetwork.o helpers.o -g -Wall -Wno-unused -O3 -std=c++0x -lm -o neuralNetMain
```

## Running the network

You have the choice between running the network on one individual training and test set or running the network on multiple training and test sets. For the examples below the training and test sets have already been generated with the generate_jackknife_sets.py file and placed in the data/ folder.

To run an individual training and test set use the -s flag (s stands for single) and specify a single *.train file followed by a *.test file.
```
./neuralNetMain -s data/iris/iris_data.4.train data/iris/iris_data.4.test
```

To run multiple training and test sets use the -d flag (d for directory) and specify a directory containing *.train and *.test file pairs
``` bash
./neuralNetMain -d data/iris/
```

## Sample run example

Here is a walk through of what happens when the neural network is ran on the Iris data from [UCI machine learning repository](http://archive.ics.uci.edu/ml/datasets/Iris) on multiple generated [Jackknife sets](https://en.wikipedia.org/wiki/Cross-validation_(statistics)) (leave-one-out cross-validation). First we begin by executing the network on all the training and test files:

``` bash
./neuralNetMain -d data/iris/
```

Then what follows is a prompt to get you to input parameters for the neural network. Here I have entered some values I think would work well with the Iris data, but this will vary on your problem. 

```
DIR: data/iris/

Please enter a positive integer for the number of hidden nodes
number of hidden nodes = 8

Please enter a positive integer for the maximum epoch:
maximum epoch = 2500

Please enter a floating point number for the amount of error until
the network is considered trained (recommended: [0.05 to 0.00001])
trained error = 0.0001

Please enter a floating point number for the learning rate 
learning rate = 0.06
```

Following this the neural network will be run on each training file until either the trained error is low enough (below the entered threshold) or the maximum epoch number is reached.

```
Epoch: 500  Training error: 0.0033523508
Epoch: 1000  Training error: 0.0022021411
Epoch: 1500  Training error: 0.0020096796
Epoch: 2000  Training error: 0.0019228072
Epoch: 2500  Training error: 0.0018578215
data/iris//iris_data.0.test - 1.0000% tests passed

Epoch: 500  Training error: 0.0033517515
Epoch: 1000  Training error: 0.0022430933
Epoch: 1500  Training error: 0.0020318430
Epoch: 2000  Training error: 0.0019374055
Epoch: 2500  Training error: 0.0018736924
data/iris//iris_data.1.test - 1.0000% tests passed

Epoch: 500  Training error: 0.0025896202
Epoch: 1000  Training error: 0.0012376670
Epoch: 1500  Training error: 0.0008850078

...

Epoch: 500  Training error: 0.0032392684
Epoch: 1000  Training error: 0.0022008781
Epoch: 1500  Training error: 0.0020510914
Epoch: 2000  Training error: 0.0019954377
Epoch: 2500  Training error: 0.0019641235
data/iris//iris_data.8.test - 1.0000% tests passed

Epoch: 500  Training error: 0.0019428469
Epoch: 1000  Training error: 0.0011091029
Epoch: 1500  Training error: 0.0009328363
Epoch: 2000  Training error: 0.0008437366
Epoch: 2500  Training error: 0.0007783890
data/iris//iris_data.9.test - 0.7143% tests passed
```

Once all the pairs of *.train and *.test files has been run you will get a statistics summary of how each pair of training and test files performed. 

```
Individual file stats:
data/iris//iris_data.0.test - 100.000000% tests passed
data/iris//iris_data.1.test - 100.000000% tests passed
data/iris//iris_data.2.test - 85.714286% tests passed
data/iris//iris_data.3.test - 100.000000% tests passed
data/iris//iris_data.4.test - 100.000000% tests passed
data/iris//iris_data.5.test - 100.000000% tests passed
data/iris//iris_data.6.test - 100.000000% tests passed
data/iris//iris_data.7.test - 100.000000% tests passed
data/iris//iris_data.8.test - 100.000000% tests passed
data/iris//iris_data.9.test - 71.428571% tests passed

You ran 10 tests.
MEAN:     0.9571
MEDIAN:   1.0000
VARIANCE: 0.0093
SD:       0.0964
```


## Creating your own datasets

If you have some of your own data and you would like to run it on this neural network you can easily do so! The generate_jackknife_sets.py file can generate *.train and *.test files given an input data file structured in the following way:

The first row of the input data should contain comma seperated column names representing the data that will show up below this name. The columns of data for the input nodes should be on the left and the expected output data columns on the right. Below the first row should follow associated rows of comma delimited data values of either real numbers or nominal data. This format is very similar to the CSV.

For examples of how the input file is setup checkout:
* [The bimodal data input file] (https://github.com/williamfiset/NeuralNetwork/blob/master/data/bimodal/bimodal_data) 
* [The Iris data input file] (https://github.com/williamfiset/NeuralNetwork/blob/master/data/iris/iris_data)
* [The spiral data] (https://github.com/williamfiset/NeuralNetwork/blob/master/data/spiral/spiral_data)

The last step is to actually generate the *.train and *.test file using the script. To generate 25 *.train and *.test jackknife files with the iris data do the following:

``` python
python generate_jackknife_sets.py 25 4
```

Note that the last argument "4" tells the script which column should be considered the cutoff point for input node data to the neural network. The iris data has a total of five columns. The first four are used as input and the last is the expected output value of the network (usually a nominal value but doesn't have to be).










