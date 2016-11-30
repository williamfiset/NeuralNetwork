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

void printAr(double *ar, int sz);
void clearArray(double *ar, int sz);

void setRandomValues(double *array, int sz);
double * createRandomArray(int sz);

double ** create_DBL_2d_array(int numRows, int numCols);
void free_DBL_2d_array(double ** arr, int numRows);
void print_DBL_2d_array(double **ar, int numRows, int numCols);

void trim( std::string &s );
std::vector <std::string> split (const std::string &str, char delim );
bool is_number(const std::string &s);

#endif
