#include <iostream>

 using namespace std;

int main(int argc, char** argv) {
  long int N = 0, K = 0;
  if (argc < 3) {
    cout << "Please, enter:\n./steps <N> <K>\n";
    return -1;
  }

  char* ptrN;
  char* ptrK;
  N = strtol(argv[1], &ptrN, 10);
  K = strtol(argv[2], &ptrK, 10);

  if (*ptrN || *ptrK) {
      cout << "Please, enter <N> <K> as integer in decimal number system.\n";
      return -1;
  }

  //Solution:
  //I made a short comment in the README

  //On each step he could steps over K stairs.
  //Also on each step he makes 1 stair "done", because he is staying on it.
  //It means that he makes "done" K + 1 stairs per 1 step.

  int result = 0;
  while (N > 0) {
    N -= 1 + K;
    ++result;
  }

  cout << result << endl;
  return 0;
}
