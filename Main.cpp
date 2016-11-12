/*
William Fiset
Machine Learning Project - Neural Network
*/

#include <iostream>
#include <cmath>
#include <limits>
#include <iomanip>

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "Helper.hpp"

using namespace std;

#define LEARNING_RATE 0.25 //0.08
// #define MOMENTUM 0.3
#define MAX_ERROR 0.0001
#define MAX_EPOCH 700000

#define NUM_INPUT_NODES 5
#define NUM_OUTPUT_NODES 6
#define NUM_HIDDEN_NODES 7

#define LAYERS 2

// This pointer will hold all the edge weights for each layer in the neural network
// Each pointer holds a reference to a compact 2D array (adjacency matrix) of edge values
// potentially of different sizes taking into account the input nodes and output nodes.
double *** networkWeights = new double**[LAYERS];

// Declare pointers to arrays which will contain values for:
// I - the input nodes
// H - the hidden nodes
// O - the output nodes
// T - the target output for the output nodes
// E - the output bias error
// F - the hidden node error
double *I, *H, *O, *T, *E, *F;

double *hiddenNodeBias, *outputNodeBias;

auto tmpValues = new double[NUM_HIDDEN_NODES][NUM_OUTPUT_NODES]();

// The sigmoid function returns a value in the range [0,1]
const inline double sigmoid(double x) {
  return 1.0 / (1.0 + exp(-x));
}

// The sigmoid error is the derivative of the sigmoid function
const inline double sigmoidError(double x) {
  return x * (1.0 - x);
}


// initializes random values to all nodes in the neural network
void initialNodeValues() {

  I = new double[NUM_INPUT_NODES];
  H = new double[NUM_HIDDEN_NODES];
  O = new double[NUM_OUTPUT_NODES];
  T = new double[NUM_OUTPUT_NODES];

  // Clear all the data in the arrays
  clearArray(I, NUM_INPUT_NODES);
  clearArray(H, NUM_INPUT_NODES);
  clearArray(O, NUM_INPUT_NODES);
  clearArray(T, NUM_INPUT_NODES);

}

// Set random bias values for all nodes
void initialNodeBiasValues() {

  hiddenNodeBias = new double[NUM_HIDDEN_NODES];
  outputNodeBias = new double[NUM_OUTPUT_NODES];

  E = new double[NUM_OUTPUT_NODES];
  F = new double[NUM_HIDDEN_NODES];

  setRandomValues(hiddenNodeBias, NUM_HIDDEN_NODES);
  setRandomValues(outputNodeBias, NUM_OUTPUT_NODES);

  clearArray(E, NUM_OUTPUT_NODES);
  clearArray(F, NUM_HIDDEN_NODES);


}

void initialEdgeWeights() {
  
  // Allocate memory for edge weights
  double ** inputNodeEdgeWeights = new double*[NUM_INPUT_NODES];
  double ** outputNodeEdgeWeights = new double*[NUM_HIDDEN_NODES];
  
  // Randomly assign values for edges between input nodes and hidden nodes
  // and also hidden nodes to iutput nodes
  for (int i = 0; i < NUM_INPUT_NODES; i++)
    inputNodeEdgeWeights[i] = createRandomArray(NUM_HIDDEN_NODES);
    
  for (int i = 0; i < NUM_HIDDEN_NODES; i++)
    outputNodeEdgeWeights[i] = createRandomArray(NUM_OUTPUT_NODES);
  
  networkWeights[0] = inputNodeEdgeWeights;
  networkWeights[1] = outputNodeEdgeWeights;

}

void deallocateNeuralNetwork() {
  
  delete [] I;
  delete [] H;
  delete [] O;
  delete [] T;
  delete [] E;
  delete [] F;
  delete [] hiddenNodeBias;
  delete [] outputNodeBias;
  
  // deallocate edge matrix
  for (int i = 0; i < 2; ++i) {
    double ** adjacencyMatrix = networkWeights[i];
    if (i == 0) {
      for (int i = 0; i < NUM_INPUT_NODES; ++i) {
        double *edges = adjacencyMatrix[i];
        delete [] edges;
      }
    } else {
      for (int i = 0; i < NUM_HIDDEN_NODES; ++i) {
        double *edges = adjacencyMatrix[i];
        delete [] edges; 
        // double *edges2 = tmpValues[i];
        // delete [] edges2;       
      }
    }
    delete [] adjacencyMatrix;
  }
  delete [] networkWeights;

}

void initializeNeuralNetwork() {
  initialNodeValues();
  initialNodeBiasValues();
  initialEdgeWeights();
}

// For each pair of layers propagate the 
// input data across the neural network 
void forwardPropagate() {

  // Input nodes to hidden nodes (layer 0 to 1)
  for (int i = 0; i < NUM_HIDDEN_NODES; i++) {

    double sum = 0.0;
    double bias = hiddenNodeBias[i];
    
    for (int j = 0; j < NUM_INPUT_NODES; j++) {
      double edgeWeight = networkWeights[0][j][i];
      sum += (I[j] * edgeWeight);
    }

    H[i] = sigmoid(sum + bias);
    
  }

  for (int i = 0; i < NUM_OUTPUT_NODES; i++) {
    
    double sum  = 0.0;
    double bias = outputNodeBias[i];
    
    for (int j = 0; j < NUM_HIDDEN_NODES; j++) {
      
      double edgeWeight = networkWeights[1][j][i];
      sum += ( edgeWeight * H[j]);
      
    }
    
    O[i] = sigmoid(sum + bias);
    
  } 

}

const inline double delta(double o, double t) {
  return (o - t) * sigmoidError(o);
}

// Got ideas from
// Andrew Hamilton Wright and
// https://mattmazur.com/2015/03/17/a-step-by-step-backpropagation-example/
double backwardsPropagate() {

  double totalSquaredError = 0.0;

  // Compute output node errors
  for (int i = 0; i < NUM_OUTPUT_NODES; ++i) {
    
    // Find total error
    double errorDiff = T[i] - O[i];
    totalSquaredError += (errorDiff * errorDiff) / 2.0;

    // Update output node bias
    E[i] = sigmoidError(O[i]) * ( errorDiff );
    outputNodeBias[i] = outputNodeBias[i] + LEARNING_RATE * E[i];

  }

  // Adjust edge weights from output layer to first hidden node layer
  for(int i = 0; i < NUM_HIDDEN_NODES; i++ ) {

    double biasSum = 0.0;
    for (int j = 0; j < NUM_OUTPUT_NODES; j++) {
      
      double edgeWeight = networkWeights[1][i][j];
      double deltaValue = delta(O[j], T[j]) * H[i];

      // Be careful not to override the old edge values, so store the new ones
      tmpValues[i][j] = edgeWeight - LEARNING_RATE * deltaValue;
      biasSum += edgeWeight * E[j];

    }

    // Adjust hidden node bias
    F[i] = sigmoidError(H[i]) * biasSum;
    hiddenNodeBias[i] = hiddenNodeBias[i] + LEARNING_RATE * F[i];
    
  }

  for (int i = 0; i < NUM_INPUT_NODES; ++i) {

    // Change the value of the edge from input node i to hidden node j
    for (int j = 0; j < NUM_HIDDEN_NODES; ++j) {
  
      // Get input edge weights
      double edgeWeight = networkWeights[0][i][j];
      double sum = 0.0;

      // Can I precompute these??
      for (int k = 0; k < NUM_OUTPUT_NODES; ++k)
        sum += delta(O[k], T[k]) * networkWeights[1][j][k];

      sum *= sigmoidError(H[j]);
      sum *= I[i];

      networkWeights[0][i][j] = edgeWeight - LEARNING_RATE * sum;

    }

  }

  // Copy values back into matrix
  for (int i = 0; i < NUM_HIDDEN_NODES; ++i)
    for (int j = 0; j < NUM_OUTPUT_NODES; ++j)
      networkWeights[1][i][j] = tmpValues[i][j];
    
  return totalSquaredError;

}

// Takes in a number and the number of bits and outputs
// a vector with the number in binary the the most significant on the left
void set(double *ptr, int num, int sz) {

  // double * out = new double[sz];
  for (int i = 0; i < sz; ++i)
    ptr[i] = (double) bitIsSet(num, sz - i - 1);

}

// Trains a neural network and returns the global error
double trainNeuralNetwork () {

  double globalError = numeric_limits<double>::max();
  
  for (int epoch = 0; epoch < MAX_EPOCH && globalError > MAX_ERROR; ++epoch) {
    
    int numTests = 20;
    // Test with 1, 4, 10, 11
    int input[]  = {0, 2, 3, 5, 6, 7, 8, 9, 12, 13, 14, 15,17,19,21,22,23,26,27,31};

    for (int i = 0; i < numTests; ++i) {
      
      int in = input[i];

      set(I, in, NUM_INPUT_NODES);
      set(T, in*2, NUM_OUTPUT_NODES);

      forwardPropagate();      
      double error = backwardsPropagate();
      globalError = globalError + error;

    }

    globalError = (globalError / (numTests * NUM_OUTPUT_NODES));

  }

  return globalError;

}

void displayEdgeWeights() {

  printf("\n");
  for (int i = 0; i < 2; ++i) {
    double ** matrix = networkWeights[i];
    if (i == 0) {
      for (int j = 0; j < NUM_INPUT_NODES; ++j) {
        printAr(matrix[j], NUM_HIDDEN_NODES);
      }
      printf("\n");
    } else  {
      for (int j = 0; j < NUM_HIDDEN_NODES; ++j) {
        printAr(matrix[j], NUM_OUTPUT_NODES);
      }
    }
  }
  printf("\n");

}


void setExample() {
    
  networkWeights[0][0][0] = 0.15; // w1
  networkWeights[0][0][1] = 0.25; // w2
  networkWeights[0][1][0] = 0.20; // w3
  networkWeights[0][1][1] = 0.30; // w4
  
  networkWeights[1][0][0] = 0.40; // w5
  networkWeights[1][0][1] = 0.50; // w6
  networkWeights[1][1][0] = 0.45; // w7
  networkWeights[1][1][1] = 0.55; // w8
  
  // Wanted output
  T[0] = 0.01;
  T[1] = 0.99;

  // Original Input values (goal is 0.01 and 0.99)
  I[0] = 0.05; // i1
  I[1] = 0.10; // i2
  
  // These are unknown values
  H[0] = 0; 
  H[1] = 0;
  O[0] = 0; 
  O[1] = 0;  
  
  hiddenNodeBias[0] = 0.35;
  hiddenNodeBias[1] = 0.35;

  outputNodeBias[0] = 0.60;
  outputNodeBias[1] = 0.60;

  
}

void trainExample() {
  for(int i = 0; i < MAX_EPOCH; i++) {
    forwardPropagate();
    backwardsPropagate();
  }
  for (int i = 0; i < NUM_OUTPUT_NODES; ++i) {
    printf("%0.9f\n", O[i]);
  }
}

int getValue() {
  int sum = 0;
  for (int i = 0; i < NUM_OUTPUT_NODES; i++) {
    if (O[i] > 0.5) {
      sum += 1 << (NUM_OUTPUT_NODES - i - 1);
    }
  }
  return sum;
}

void testNeuralNetwork() {
  
  for (int i = 0; i < 32; i++) {
    set(I, i, NUM_INPUT_NODES);
    forwardPropagate();
    printf("%d %d\n", i, getValue());
  }

}

int main(int argc, char const * argv[]) {



  initializeNeuralNetwork();
  
  // setExample();
  // displayEdgeWeights();
  // trainExample();
  // displayEdgeWeights();

  trainNeuralNetwork();
  testNeuralNetwork();
  deallocateNeuralNetwork();

  return 0;

}


