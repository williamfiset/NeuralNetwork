
#include "helpers.hpp"

// Return a real number between zero and one
const inline double randomNum() {
  return ((double)rand() / RAND_MAX);
}

// Set random values in an array to be in between the range zero to one
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

// Stolen from
// http://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
bool is_number(const std::string &s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

