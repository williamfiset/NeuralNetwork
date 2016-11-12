#include <iostream>
#include <fstream>

#include "helpers.hpp"
#include "neuralnetwork.hpp"

using namespace std;

#define MAX_EPOCH 25000
#define MAX_ERROR 0.00015
#define LEARNING_RATE 0.25

void readInput( ifstream &stream, double **inputData, double **expectedOutput, int numRows, int numInputCols, int numOutputCols ) {

  double num;

  for (int i = 0; i < numRows; ++i) {
    
    // Store input column values
    for (int j = 0; j < numInputCols; ++j) {
      stream >> num;
      inputData[i][j] = num;
    }
    
    // Store output column values
    for (int j = 0; j < numOutputCols; ++j) {
      stream >> num;
      expectedOutput[i][j] = num;    
    }

  }

}

// Read file contents and initialize all variables
bool readFileContents( string &filename, int *numRows, int *numCols, int *numInputCols, int *numOutputCols, string **labels, double ***in, double ***expected ) {

  ifstream stream;
  stream.open(filename);

  // Try opening file
  if (stream) {

    stream >> *numRows >> *numCols >> *numInputCols;
    *numOutputCols = *numCols - *numInputCols;
    
    string labelName;
    *labels = new string[*numCols];
    
    // Read label names
    for (int i = 0; i < *numCols; ++i) {
      stream >> labelName;
      (*labels)[i] = labelName;
    }

    // init memory 
    *in       = create_DBL_2d_array( *numRows, *numInputCols  );
    *expected = create_DBL_2d_array( *numRows, *numOutputCols );

    readInput( stream, *in, *expected, *numRows, *numInputCols, *numOutputCols );
    stream.close();

    return true;

  }

  return false;

}

void startNeuralNet(string &trainingFileName, string &testFileName) {

  string *labels = NULL;
  int numRows, numCols, numInputCols, numOutputCols, numHiddenNodes;
  double ** trainingInputData, ** testInputData;
  double ** trainingExpectedOutput, ** testExpectedOutput;

  if ( readFileContents(trainingFileName, &numRows, &numCols, &numInputCols, &numOutputCols, &labels, &trainingInputData, &trainingExpectedOutput) ) {

    printf("How many hidden nodes would you like?\n");
    printf("Enter a positive integer: ");
    cin >> numHiddenNodes;

    NeuralNetwork net( numInputCols, numHiddenNodes, numOutputCols, MAX_EPOCH, LEARNING_RATE, MAX_ERROR );
    net.train(trainingInputData, trainingExpectedOutput, numRows);

    // Free memory
    // free_DBL_2d_array(trainingInputData, numRows);
    // free_DBL_2d_array(trainingExpectedOutput, numRows);

    if ( readFileContents(testFileName, &numRows, &numCols, &numInputCols, &numOutputCols, &labels, &testInputData, &testExpectedOutput) ) {

      net.test( testInputData, testExpectedOutput, numRows );

      // Free memory
      free_DBL_2d_array(testInputData, numRows);
      free_DBL_2d_array(testExpectedOutput, numRows);

    } else {
      cout << "Oops something went wrong opening '" << testFileName << "'" << endl;
    }

    delete [] labels;

  } else {
    cout << "Oops something went wrong opening '" << trainingFileName << "'" << endl;
  }


  // output the standard deviation and varience and other stats

}

void printHelp() {
  printf("\nHELP MENU:\n");
  printf("./NeuralNetMain [TRAINING_FILE][TEST_FILE]\n\n");
}

int main(int num_arguments, char const *argv[]) {

  // Seed random number generator
  srand(time(NULL));

  if (num_arguments == 3) {

    string trainingFileName = string(argv[1]);
    string testFileName = string(argv[2]);

    startNeuralNet(trainingFileName, testFileName);

  } else {
    cout << "Error not enough arguments" << endl;
    printHelp();
  }
  
  return 0;
}
