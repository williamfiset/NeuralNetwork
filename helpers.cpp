
#include "helpers.hpp"

// Return a data between [0,1]
const inline double randomNum() {
  return ((double)rand() / RAND_MAX);
}

void printAr(double *ar, int sz) {
	for (int i = 0; i < sz; i++)
		printf("%0.4f ", ar[i]);
	printf("\n");
}

// Set random values in the array between the range [0,1]
void setRandomValues(double *array, int sz) {
  if (array != NULL) {
    for (int i = 0; i < sz; ++i) {
      array[i] = randomNum();
    }
  }
}

// Initialize the values of an array to zero. This is often used
// to remove junk values from recently allocated memory
void clearArray(double *ar, int sz) {
  if (ar != NULL) {
    for(int i = 0; i < sz; i++) {
      ar[i] = 0;
    }
  }
}

void print_DBL_2D_array(double **ar, int numRows, int numCols) {
  if (ar != NULL) {
    for (int i = 0; i < numRows; ++i) {
      for (int j = 0; j < numCols; ++j)
        printf("%0.4f ", ar[i][j]);
      printf("\n");
    }
  }
}

// Allocate memory for a 2d array and return a pointer to that memory
double ** create_DBL_2d_array(int numRows, int numCols) {
  double ** ar = new double*[numRows];
  for (int i = 0; i < numRows; ++i)
    ar[i] = new double[numCols];
  return ar;
}

// Deallocate the memory allocated to a 2d array
void free_DBL_2d_array(double ** arr, int numRows) {
  if (arr != NULL) {
    for (int i = 0; i < numRows; ++i)
      delete [] arr[i];  
    delete [] arr;
  }
} 

// Split a string with a specified delimeter
std::vector <std::string> split (const std::string &str, char delim ) {
  std::vector<std::string> elems;
  std::stringstream ss(str);
  std::string item;
  while (getline(ss, item, delim))
    if (!item.empty()) // We do not want empty strings
      elems.push_back(item);
  return elems;
}


// Trims a string removing whitespace on left and right
void trim( std::string &s ) {
  int left = 0, right = s.size()-1, sz = s.size();
  for (; left != sz && isspace(s[left]); left++) { }
  for (; right >= 0 && isspace(s[right]); right--) { }
  s = s.substr(left, right - left + 1 ); // substr returns a copy 
}



