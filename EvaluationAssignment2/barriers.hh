#pragma once

#include <cstdlib>
#include <atomic>
#include <iostream>
#include <vector>
#include <math.h>

typedef int int_t;

using namespace std;

class Centralized{
  int_t numThreads;
  atomic<int_t> counter;
  bool sense;

  Centralized(int_t num) : numThreads(num),counter(0),sense(false) {};
  void operator() (int_t threadID);
};

class Dissemination{
  int_t numThreads;
  int_t numRounds;
  bool sense;
  int_t parity;
  vector<vector<vector<bool>>> flags;

  Dissemination(int_t num);
  void operator() (int_t threadID);
};

class MCS{
  int_t numThreads;

  MCS(int_t num);
  void operator() (int_t threadID);
};
