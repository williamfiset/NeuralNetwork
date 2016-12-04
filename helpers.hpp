#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <algorithm>

// Set all the values in an array to be zero (cleaner than memset IMO)
void clearArray(double *ar, int sz);

// Set random values in the array between the range [0,1]
void setRandomValues(double *array, int sz);

// Allocates memory for a 2D double array
double ** create_DBL_2d_array(int numRows, int numCols);

// Deallocate all the memory in a 2D double array
void free_DBL_2d_array(double ** arr, int numRows);

// Determines is a string is a number
bool is_number(const std::string &s);

// Split a string by a delimiter
std::vector <std::string> split (const std::string &str, char delim );

#endif
