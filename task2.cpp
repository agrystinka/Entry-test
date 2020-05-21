#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  long int N = 0, D = 0;
  if (argc < 3) {
    cout << "Please, enter:\n./cables <N> <D>" << endl;
    return -1;
  }

  char* ptrN;
  char* ptrK;
  N = strtol(argv[1], &ptrN, 10);
  D = strtol(argv[2], &ptrK, 10);

  if (*ptrN || *ptrK) {
      cout << "Please, enter <N> <D> as integer in decimal number system.\n";
      return -1;
  }

  //Solution:

  //A1 = D, A2 = 2*D, A3 = 3*D, AN = N*D - it is arithmetic progression
  //Total cable lenth is sum of N first numbers of arithmetic sequence
  //S(N) = (((2 * A1 + (N - 1)* D) / 2) * N
  //For the task A1 = D, then S(N) = (N + 1) * D * N / 2

  //Division by 2 could be replaced with a binary shift to the right
  //in order to increase the performance rate

  cout << (((N + 1) * D * N) >> 1 ) << endl;

  return 0;
}
