#pragma once

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BILLION  1000000000;

typedef int32_t int_t;
typedef double bound_t;
typedef double timed_t;

typedef int64_t vec_t;     //this is the type our coordinates/velocitys will have

void generate_random_list(vec_t array[], bound_t bound, int_t size);
void update_coords(vec_t xs[], vec_t ys[], vec_t zs[], vec_t vx[], vec_t vy[], vec_t vz[], int_t size);

vec_t sum_array(vec_t array[], int_t size);
