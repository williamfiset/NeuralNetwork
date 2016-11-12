#include <iostream>
#include "helpers.hpp"
#include "neuralnetwork.hpp"
using namespace std;

void set(string arr[3]) {
  arr[0] = "a";
  arr[1] = "b";
  arr[2] = "c";
}

void f(double ***ptr) {

}

void g(string **l) {
  *l = new string[2];
  (*l)[0] = "Will";
  (*l)[1] = "log";
}

int main(int argc, char const *argv[]) {

  string *labels = NULL;
  g(&labels);
  cout << labels[0] << endl;
  cout << labels[1] << endl;

  // srand(time(NULL));
  // for (int i = 0; i < 100; ++i) {
  //   cout << randomNum() << endl;
  // }

  // double ** p;
  // f(&p);

  // cout << p[0][0] << endl;
  // cout << p[0][1] << endl;
  // cout << p[1][0] << endl;
  // cout << p[1][1] << endl;

  return 0;
}
