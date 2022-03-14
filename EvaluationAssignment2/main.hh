#pragma once

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

#include <time.h>

typedef int64_t int_t;

#define NANOS 1000000000

double timer(int_t length, double, int_t secondArraySize, int_t prefetch);
