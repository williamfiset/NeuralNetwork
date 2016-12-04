#include <iostream>
#include <fstream>
#include <glob.h>
#include <map>

#include "helpers.hpp"
#include "neuralnetwork.hpp"

// If toggled true then display text verbosely
bool VERBOSE = false;

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

  MAX_ERROR = 0.00001;
  // printf("\nPlease enter a floating point number for the amount of error until\n");
  // printf("the network is considered trained (recommended: [0.05 to 0.00001])\n");
  // printf("trained error = ");
  // cin >> MAX_ERROR;

  printf("\nPlease enter a floating point number for the learning rate \n");
  printf("learning rate = ");
  cin >> LEARNING_RATE;

}

void displayStats(std::vector<double> &dataPoints, std::vector<string> &testFileNames) {

  int len = dataPoints.size();

  if ( dataPoints.size() != testFileNames.size()) {
    cout << dataPoints.size() << " " << testFileNames.size() << endl;
    printf("Error. Number of dataPoints is not equal to number of test files\n");
    return;
  }

  printf("\nIndividual file stats:\n");
  for (int i = 0; i < len; ++i)
    printf("%s - %f%% tests passed\n", testFileNames[i].c_str(), (100*dataPoints[i]) );
  printf("\n");
  

  if (len > 0) {

    double median, mean, variance, SD;
    median = mean = variance = SD = 0;
    
    // Sort the test scores. Important for median.
    sort(dataPoints.begin(), dataPoints.end());

    // Find the median
    if (len % 2 == 0) {
      median = (dataPoints[len/2] + dataPoints[len/2 - 1]) / 2.0;
    } else {
      median = dataPoints[len/2];
    }

    // Find the mean
    double sum = 0.0;
    for (int i = 0; i < len; ++i)
      sum += dataPoints[i];
    mean = sum / len;

    // Find variance
    for (int i = sum = 0; i < len; ++i) {
      double score = dataPoints[i];
      sum += (mean - score) * (mean - score);
    }

    // Handle special case of one test point
    if (len == 1)
      variance = dataPoints[0];
    else
      variance = sum / (len-1);
    
    SD = sqrt(variance);

    printf("You ran %d tests.\n", len );
    printf("MEAN:     %0.4f\n", mean );
    printf("MEDIAN:   %0.4f\n", median );
    printf("VARIANCE: %0.4f\n", variance );
    printf("SD:       %0.4f\n", SD );

  } else {
    printf("No stats to display. Please run a jackknife set on the neural network to get statistics\n");
  }

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
void executeNeuralNet(string &trainingFileName, string &testFileName, vector<double> &dataPoints) {

  string *labels = NULL;
  int numRows, numCols, numInputCols, numOutputCols;
  double ** trainingInputData, ** testInputData;
  double ** trainingExpectedOutput, ** testExpectedOutput;

  if ( readFileContents(trainingFileName, &numRows, &numCols, &numInputCols, &numOutputCols, &labels, &trainingInputData, &trainingExpectedOutput) ) {

    // Initialize and train neural network
    NeuralNetwork net( numInputCols, NUM_HIDDEN_NODES, numOutputCols, MAX_EPOCH, LEARNING_RATE, MAX_ERROR );
    net.train(trainingInputData, trainingExpectedOutput, numRows);

    // Free memory
    free_DBL_2d_array(trainingInputData, numRows);
    free_DBL_2d_array(trainingExpectedOutput, numRows);

    if ( readFileContents(testFileName, &numRows, &numCols, &numInputCols, &numOutputCols, &labels, &testInputData, &testExpectedOutput) ) {

      // Test neural network against test data
      double score = net.test( testInputData, testExpectedOutput, numRows );
      // printf("%s - %.4f%% tests passed\n\n", testFileName.c_str() , score);
      dataPoints.push_back(score);

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
  
  if (VERBOSE) printf("Searching in '%s' for *.train and *.text files\n", directory.c_str() );

  map <string, string> trainingMap;
  map <string, string> testMap;
  
  glob_t glob_result;
  glob( (directory + "/*").c_str(), GLOB_TILDE, NULL, &glob_result);
  
  vector<string> testFileNames;

  // Loop through all files in the directory
  for(unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
    
    string dirItem = string(glob_result.gl_pathv[i]);
    
    vector<string> dirItemSplit = split(dirItem, '.');
    int N = dirItemSplit.size();
    
    if (N >= 3) {

      string fileName = dirItemSplit[0];
      string fileExtension = dirItemSplit[N-1];
      string fileNumber = dirItemSplit[N-2];
      
      if ( is_number(fileNumber) ) {
        if (fileExtension == "train") {
          trainingMap[fileNumber] = dirItem;
        } else if (fileExtension == "test") {
          testFileNames.push_back(dirItem);
          testMap[fileNumber] = dirItem;
        }
      }
      
    }
  }
  
  if (VERBOSE) printf("Found %ld training files\n", trainingMap.size() );
  if (VERBOSE) printf("Found %ld testing files\n", testMap.size() );

  if (trainingMap.size() > 0) {

    setNeuralNetworkParameters();
    std::vector<double> dataPoints;

    // Find all pairs of training and testing files and
    // run them against the neural net
    map<string, string>::iterator it = trainingMap.begin();
    for( ; it != trainingMap.end(); it++ ) {
      
      string fileNumber = it->first;
      string trainingFileName = it->second;
      
      // Associated test file exists
      if ( testMap.count(fileNumber) > 0 ) {
        
        string testFileName = testMap[fileNumber];
        executeNeuralNet( trainingFileName, testFileName, dataPoints );
        
      }
    }

    displayStats(dataPoints, testFileNames);

  }

  
}

void printHelp() {
  
  printf("\nHELP MENU:\n");
  printf("./NeuralNetMain [-d DIR][-s TRAINING_FILE TEST_FILE]\n");
  printf("-d     Specify the directory containing *.train and *.test files\n");
  printf("-s     Single test. Specify a *.train and a *.test file pair\n");
  printf("-v     Turn on verbose mode to display more information\n");
  printf("-h     Print this help menu\n\n");

}

int main(int num_arguments, char const *argv[]) {

  if (num_arguments == 1) {
    printf("Please supply command-line arguments!\n");
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

          std::vector<double> dataPoints;
          std::vector<string> testFileNames;
          testFileNames.push_back(testFileName);

          executeNeuralNet(trainingFileName, testFileName, dataPoints);
          displayStats(dataPoints, testFileNames);
        
        } else {
          cout << "-s requires two arguments, namely '-s TRAINING_FILE TEST_FILE'" << endl;
        }
      
      // Turn on verbose mode!
      } else if ( option == "v" ) {

        VERBOSE = true;

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
