#include <iostream>
#include <fstream>
#include <glob.h>
#include <map>

#include "helpers.hpp"
#include "neuralnetwork.hpp"

using namespace std;

int MAX_EPOCH, NUM_HIDDEN_NODES;
double MAX_ERROR, LEARNING_RATE;

void setNeuralNetworkParameters() {

  printf("\nPlease enter a positive integer for the number of hidden nodes\n");
  printf("number of hidden nodes = ");
  cin >> NUM_HIDDEN_NODES;

  printf("\nPlease enter a positive integer for the maximum epoch:\n");
  printf("maximum epoch = ");
  cin >> MAX_EPOCH;

  printf("\nPlease enter a floating point number for the amount of error until\n");
  printf("the network is considered trained (recommended: [0.05 to 0.00001])\n");
  printf("trained error = ");
  cin >> MAX_ERROR;

  printf("\nPlease enter a floating point number for the learning rate \n");
  printf("learning rate = ");
  cin >> LEARNING_RATE;

}

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

// Executes the neural network on some input. The parameters to the nerual
// netork must be set before calling this function ( NUM_HIDDEN_NODES, MAX_EPOCH, LEARNING_RATE, MAX_ERROR ... )
void executeNeuralNet(string &trainingFileName, string &testFileName) {

  string *labels = NULL;
  int numRows, numCols, numInputCols, numOutputCols;
  double ** trainingInputData, ** testInputData;
  double ** trainingExpectedOutput, ** testExpectedOutput;

  if ( readFileContents(trainingFileName, &numRows, &numCols, &numInputCols, &numOutputCols, &labels, &trainingInputData, &trainingExpectedOutput) ) {

    // Intialize and train neural network
    NeuralNetwork net( numInputCols, NUM_HIDDEN_NODES, numOutputCols, MAX_EPOCH, LEARNING_RATE, MAX_ERROR );
    net.train(trainingInputData, trainingExpectedOutput, numRows);

    // Free memory
    free_DBL_2d_array(trainingInputData, numRows);
    free_DBL_2d_array(trainingExpectedOutput, numRows);

    if ( readFileContents(testFileName, &numRows, &numCols, &numInputCols, &numOutputCols, &labels, &testInputData, &testExpectedOutput) ) {

      // Test neural network against test data
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
// and run them on the neural network
void runMultipleTests(string &directory) {
  
  map <string, string> trainingMap;
  map <string, string> testMap;
  
  glob_t glob_result;
  glob( (directory + "/*").c_str(), GLOB_TILDE, NULL, &glob_result);

  // Loop through all files in the directory
  for(unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
    
    string dirItem = string(glob_result.gl_pathv[i]);
    
    vector<string> dirItemSplit = split(dirItem, '.');
    int N = dirItemSplit.size();
    
    if (N >= 3) {

      // string fileName = dirItemSplit[0];
      string fileExtension = dirItemSplit[N-1];
      string fileNumber = dirItemSplit[N-2];
      
      if ( is_number(fileNumber) ) {
        if (fileExtension == "train") {
          trainingMap[fileNumber] = dirItem;
        } else if (fileExtension == "test") {
          testMap[fileNumber] = dirItem;
        }
      }
      
    }
  }
  
  if (trainingMap.size() > 0) {

    setNeuralNetworkParameters();

    // Find all pairs of training and testing files and
    // run them against the neural net
    map<string, string>::iterator it = trainingMap.begin();
    for( ; it != trainingMap.end(); it++ ) {
      
      string fileNumber = it->first;
      string trainingFileName = it->second;
      
      // Associated test file exists
      if ( testMap.count(fileNumber) > 0 ) {
        
        string testFileName = testMap[fileNumber];
        executeNeuralNet( trainingFileName, testFileName );
        
      }
    }

  }

  
}

void printHelp() {
  printf("\nHELP MENU:\n");
  printf("./NeuralNetMain [-d DIR][-s TRAINING_FILE TEST_FILE]\n");
  printf("-d     Specify the directory containing *.train and *.test files\n");
  printf("-s     Single test. Specify a *.train and a *.test file pair\n");
  printf("-h     Print this help menu\n\n");
}

int main(int num_arguments, char const *argv[]) {

  if (num_arguments == 1) {
    printf("Please supply commandline arguments!\n");
    printHelp();
    return 0;
  }

  // Seed random number generator
  srand(time(NULL));

  for (int i = 1; i < num_arguments; i++ ) {
    
    string arg = string(argv[i]);

    // Commandline option
    if (arg[0] == '-') {
    
      string option = arg.substr(1, arg.size());

      // Directory option containing *.#.train and *.#.test files
      if (option == "d") {
        
        if ( (i+1) < num_arguments) {
          
          string dir = string(argv[++i]);
          cout << "DIR: " << dir << endl;
          runMultipleTests(dir);
          
        } else {
          cout << "-d requires one argument, namely '-d DIR'" << endl;
        }
      
      // Individual file test
      } else if (option == "s") {
        
        if ( (i+2) < num_arguments ) {

          string trainingFileName = string(argv[++i]);
          string testFileName = string(argv[++i]);
          
          setNeuralNetworkParameters();
          executeNeuralNet(trainingFileName, testFileName);
          
        } else {
          cout << "-s requires two arguments, namely '-s TRAINING_FILE TEST_FILE'" << endl;
        }
      
      // Display help menu
      } else if (option == "h") {
          
        printHelp();

      // Unknown argument
      } else {
        printf("Unknown argument '%s'\n", arg.c_str() );
        printHelp();
      }
    
    }
        
  }
  
  return 0;
}
