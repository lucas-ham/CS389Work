#pragma once

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>
#include <functional>
#include <random>

#include <time.h>
#include <math.h>

typedef int64_t int_t;

#define NANOS 1000000000

class Timer{
  public:
    int_t length;
    double searches;
    int_t secondArraySize;
    int_t prefetch;

    Timer(int_t length, double searches,int_t secondArraySize, int_t prefetch) :
    length(length), searches(searches), secondArraySize(secondArraySize), prefetch(prefetch){};

    void operator() (double* answer);
  };

int_t calculatepower(int_t start, int_t end);

using namespace std;
