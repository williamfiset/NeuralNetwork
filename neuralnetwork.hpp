#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <iostream>
#include <cmath>
#include <limits>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include "helpers.hpp"

#define LAYERS 2
#define MAX_ACCEPTED_ERR 0.5

class NeuralNetwork {

  private:

    // constants
    double MAX_ERROR, LEARNING_RATE;
    int NUM_INPUT_NODES, NUM_HIDDEN_NODES, NUM_OUTPUT_NODES, MAX_EPOCH;

    // This pointer will hold all the edge weights for each layer in the neural network
    // Each pointer holds a reference to a compact 2D array (adjacency matrix) of edge values
    // potentially of different sizes taking into account the input nodes and output nodes.
    double *** networkWeights;

    // Declare pointers to arrays which will contain values for:
    // I - the input nodes
    // H - the hidden nodes
    // O - the output nodes
    // T - the target output for the output nodes
    // E - the output bias error
    // F - the hidden node error
    double *I, *H, *O, *T, *E, *F;

    // These will contain data about the bias on each node
    double *hiddenNodeBias, *outputNodeBias;

    // Used in back propagation method 
    double ** tmpValues;

    const inline double sigmoid(double x);
    const inline double sigmoidError(double x);
    const inline double delta(double o, double t);

    // These methods initialize the neural network's data
    void initNodeValueArrays();
    void initEdgeWeights();
    void initNodeBiasValues();

    void   forwardPropagate();
    double backwardsPropagate();

  public:
    
    // Train the network on some input data. Returns the global error after training
    double train(double ** inputData, double **expectedOutput, int numRows);
    
    // Test the network on some input data. Returns a score of the ratio of tests passed
    double test(double ** inputData, double **expectedOutput, int numRows);

    // Constructor
    NeuralNetwork(int numInputNodes,
                  int numHiddenNodes,
                  int numOutputNodes,
                  int maxEpoch,
                  double learningRate,
                  double maxError);

    // Deconstructor
    ~NeuralNetwork();
    
};

#endif
