#pragma once

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef int8_t vec_t;
typedef int8_t int_t;

void generate_random_list(vec_t array[], double bound, int_t size);

void update_coords(vec_t xs[], vec_t ys[], vec_t zs[], vec_t vx[], vec_t vy[], vec_t vz[], int_t size);

double sum_array(vec_t array[], int_t size);
