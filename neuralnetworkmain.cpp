#include <iostream>
#include <fstream>
#include <glob.h>
#include <map>

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

void executeNeuralNet(string &trainingFileName, string &testFileName) {

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
    free_DBL_2d_array(trainingInputData, numRows);
    free_DBL_2d_array(trainingExpectedOutput, numRows);

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

}

// Find all files that end in .#.train and .#.test
// and run them onthe neural network
void runMultipleTests(string &directory) {
  
  map <string, string> trainingMap;
  map <string, string> testMap;
  
  glob_t glob_result;
  glob( (directory + "/*").c_str(), GLOB_TILDE, NULL, &glob_result);

  // Loop through all files in the directory
  for(unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
    
    string dirItem = string(glob_result.gl_pathv[i]);
    cout << dirItem << endl;
    
    vector<string> dirItemSplit = split(dirItem, '.');
    int N = dirItemSplit.size();
    
    if (N >= 3) {

      // string fileName = dirItemSplit[0];
      string fileNumber = dirItemSplit[N-2];
      string fileExtension = dirItemSplit[N-1];
      
      if ( is_number(fileNumber) ) {
        if (fileExtension == "train") {
          trainingMap[fileNumber] = dirItem;
        } else if (fileExtension == "test") {
          testMap[fileNumber] = dirItem;
        }
      }
      
    }
  }
  
  map<string, string>::iterator it = trainingMap.begin();
  for( ; it != trainingMap.end(); it++ ) {
    
    string fileNumber = it->first;
    string trainingFileName = it->second;
    
    if ( testMap.count(fileNumber) > 0 ) {
      
      string testFileName = testMap[fileNumber];
      executeNeuralNet( trainingFileName, testFileName );
      
    }
  }
  
}

void printHelp() {
  printf("\nHELP MENU:\n");
  printf("./NeuralNetMain [-d DIR][-s TRAINING_FILE TEST_FILE]\n");
  printf("-d     Specify the directory containing *.train & *.test files\n");
  printf("-s     Single test. Specify a .train and a .test file\n\n");
}

int main(int num_arguments, char const *argv[]) {

  // Seed random number generator
  srand(time(NULL));

  for (int i = 1; i < num_arguments; i++ ) {
    
    string arg = string(argv[i]);
    cout << arg << endl;
    // Commandline option
    if (arg[0] == '-') {
    
      string option = arg.substr(1, arg.size());
      if (option == "d") {
        
        if ( (i+1) < num_arguments) {
          
          string dir = string(argv[++i]);
          cout << "DIR: " << dir << endl;
          runMultipleTests(dir);
          
        } else {
          cout << "-d requires one argument, namely '-d DIR'" << endl;
        }
        
      } else if (option == "s") {
        
        if ( (i+2) < num_arguments ) {

          string trainingFileName = string(argv[++i]);
          string testFileName = string(argv[++i]);

          executeNeuralNet(trainingFileName, testFileName);
          
        } else {
          cout << "-s requires two arguments, namely '-s TRAINING_FILE TEST_FILE'" << endl;
        }
        
      } else {
        printf("Unknown argument '%s'\n", arg.c_str() );
        printHelp();
      }
    
    }
        
  }
  
  return 0;
}
