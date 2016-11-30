
#include "helpers.hpp"
#include "neuralnetwork.hpp"

NeuralNetwork::NeuralNetwork(int numInputNodes,
                             int numHiddenNodes,
                             int numOutputNodes,
                             int maxEpoch,
                             double learningRate,
                             double maxError) {

  NUM_INPUT_NODES  = numInputNodes;
  NUM_HIDDEN_NODES = numHiddenNodes;
  NUM_OUTPUT_NODES = numOutputNodes;

  LEARNING_RATE = learningRate;
  MAX_EPOCH = maxEpoch;
  MAX_ERROR = maxError;

  initNodeValueArrays();
  initEdgeWeights();
  initNodeBiasValues();

  tmpValues = create_DBL_2d_array(NUM_HIDDEN_NODES, NUM_OUTPUT_NODES);

}

const inline double NeuralNetwork::sigmoid(double x) {
  return 1.0 / (1.0 + exp(-x));
}

const inline double NeuralNetwork::sigmoidError(double x) {
  return x * (1.0 - x);
}

const inline double NeuralNetwork::delta(double o, double t) {
  return (o - t) * sigmoidError(o);
}

void NeuralNetwork::initNodeValueArrays() {

  I = new double[NUM_INPUT_NODES];
  H = new double[NUM_HIDDEN_NODES];
  O = new double[NUM_OUTPUT_NODES];
  T = new double[NUM_OUTPUT_NODES];

  // Clear all the data in the arrays
  clearArray(I, NUM_INPUT_NODES);
  clearArray(H, NUM_HIDDEN_NODES);
  clearArray(O, NUM_OUTPUT_NODES);
  clearArray(T, NUM_OUTPUT_NODES);

}

void NeuralNetwork::initEdgeWeights() {

  networkWeights = new double**[LAYERS];

  // Allocate memory for edge weights
  double ** inputNodeEdgeWeights  = new double*[NUM_INPUT_NODES];
  double ** outputNodeEdgeWeights = new double*[NUM_HIDDEN_NODES];
  
  // Randomly assign values for edges values between input nodes and hidden nodes
  for (int i = 0; i < NUM_INPUT_NODES; i++) {
    double * values = new double[NUM_HIDDEN_NODES];
    setRandomValues(values, NUM_HIDDEN_NODES);
    inputNodeEdgeWeights[i] = values;
  }

  // Randomly assign values for edges values between hidden nodes and output nodes
  for (int i = 0; i < NUM_HIDDEN_NODES; i++) {
    double * values = new double[NUM_OUTPUT_NODES];
    setRandomValues(values, NUM_OUTPUT_NODES);
    outputNodeEdgeWeights[i] = values;
  }
  
  networkWeights[0] = inputNodeEdgeWeights;
  networkWeights[1] = outputNodeEdgeWeights;

}

void NeuralNetwork::initNodeBiasValues() {

  hiddenNodeBias = new double[NUM_HIDDEN_NODES];
  outputNodeBias = new double[NUM_OUTPUT_NODES];

  E = new double[NUM_OUTPUT_NODES];
  F = new double[NUM_HIDDEN_NODES];

  setRandomValues(hiddenNodeBias, NUM_HIDDEN_NODES);
  setRandomValues(outputNodeBias, NUM_OUTPUT_NODES);

  clearArray(E, NUM_OUTPUT_NODES);
  clearArray(F, NUM_HIDDEN_NODES);

}

// For each pair of layers propagate the 
// input data across the neural network   
void NeuralNetwork::forwardPropagate() {

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

// Got ideas on how to backpropagate from Dr. Andrew Hamilton Wright and
// https://mattmazur.com/2015/03/17/a-step-by-step-backpropagation-example/
double NeuralNetwork::backwardsPropagate() {

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
  
  // Precompute delta values for speed optimization
  double deltas[NUM_HIDDEN_NODES];
  for (int j = 0; j < NUM_HIDDEN_NODES; j++) {
    double sum = 0.0;
    for (int k = 0; k < NUM_OUTPUT_NODES; ++k)
      sum += delta(O[k], T[k]) * networkWeights[1][j][k];    
    deltas[j] = sum;
  }

  for (int i = 0; i < NUM_INPUT_NODES; ++i) {

    // Change the value of the edge from input node i to hidden node j
    for (int j = 0; j < NUM_HIDDEN_NODES; ++j) {
  
      // Get input edge weights
      double edgeWeight = networkWeights[0][i][j];
      double sum = deltas[j];
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

double NeuralNetwork::train(double **inputData, double **expectedOutput, int numRows) {

  double globalError = 9999999.0;
  const int NUM_TESTS = NUM_INPUT_NODES * numRows;
  
  for (int epoch = 1; epoch <= MAX_EPOCH && globalError > MAX_ERROR; ++epoch) {
    
    printf("Epoch: %d  Training error: %.10f\n", epoch, globalError);

    // Loop through each row in the training data
    for (int i = 0; i < numRows; ++i) {
      
      // set I (input) and T (target) values here
      memcpy( I, inputData[i], NUM_INPUT_NODES * sizeof(double) );
      memcpy( T, expectedOutput[i], NUM_OUTPUT_NODES * sizeof(double) );

      forwardPropagate();      
      double error = backwardsPropagate();
      
      globalError = globalError + error;

    }
    
    globalError = (globalError / (NUM_TESTS * NUM_OUTPUT_NODES));

  }

  return globalError;

}

double NeuralNetwork::test(double ** inputData, double **expectedOutput, int numRows) {

  if (numRows > 0) {

    int totalTests = numRows;
    int testsPassed = 0;

    for (int i = 0; i < numRows; ++i) {
      
      // set I (input) and T (target) values here
      memcpy( I, inputData[i], NUM_INPUT_NODES * sizeof(double) );
      memcpy( T, expectedOutput[i], NUM_OUTPUT_NODES * sizeof(double) );

      forwardPropagate();
      bool passedTest = true;

      // Extract answer from output nodes are the forward propagation
      for (int j = 0; j < NUM_OUTPUT_NODES; ++j) {

        double networkOutput = O[j];
        double expected      = expectedOutput[i][j];

        if ( fabs(expected - networkOutput) > MAX_ACCEPTED_ERR ) {
          passedTest = false;
          break;
        }

      }

      if (passedTest) testsPassed++;

    }

    double score = testsPassed / totalTests;
    return score;
  
  } else {
    return -1;
  }

}

// Deconstructor.
// Deallocate all the memory used inside the neural network
NeuralNetwork::~NeuralNetwork() {

  delete [] I;
  delete [] H;
  delete [] O;
  delete [] T;
  delete [] E;
  delete [] F;
  delete [] hiddenNodeBias;
  delete [] outputNodeBias;
 
  free_DBL_2d_array(tmpValues, NUM_HIDDEN_NODES);

  free_DBL_2d_array(networkWeights[0], NUM_INPUT_NODES);
  free_DBL_2d_array(networkWeights[1], NUM_HIDDEN_NODES);

}


