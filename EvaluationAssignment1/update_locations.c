
#include "main.h"

void generate_random_list(vec_t array[], double bound, int_t size){
  double range = 2*bound;
  double div = RAND_MAX/range;
  double min = -bound;
  for (int_t i = 0; i < size; i++){
    array[i] = min + (rand()/div);
  }
}

void update_coords(vec_t xs[], vec_t ys[], vec_t zs[], vec_t vx[], vec_t vy[], vec_t vz[], int_t size) {
  for (int_t i = 0; i < size; i ++){
    xs[i] = xs[i] + vx[i];
    ys[i] = ys[i] + vy[i];
    zs[i] = zs[i] + vz[i];
  }
}

vec_t sum_array(vec_t array[], int_t size){
  double sum = 0;
  for (int_t i = 0; i < size; i ++){
    sum += array[i];
  }
  return sum;
}


int main(int argc, char *argv[]){
  int_t size;
  int_t iters;
  bound_t position_bound = 1000;
  bound_t velocity_bound = 1;
  vec_t checksum;
  timed_t total_time;
  timed_t per_move_time;

  if (argc != 3){
    printf("Required arguements: vector_length(N) and iterations_num(M) \n");
    return 0;
  }else {
    size = atoi(argv[1]);
    iters = atoi(argv[2]);
  }

  srand(size);

  vec_t xs[size];
  generate_random_list(xs,position_bound, size);

  vec_t ys[size];
  generate_random_list(ys,position_bound,size);

  vec_t zs[size];
  generate_random_list(zs,position_bound,size);

  vec_t vx[size];
  generate_random_list(vx,velocity_bound,size);

  vec_t vy[size];
  generate_random_list(vy,velocity_bound,size);

  vec_t vz[size];
  generate_random_list(vz,velocity_bound, size);

  struct timespec start, finish, delta;

  clock_gettime(CLOCK_REALTIME, &start);
  update_coords(xs,ys,zs,vx,vy,vz, size);
  clock_gettime(CLOCK_REALTIME, &finish);

  delta.tv_sec = finish.tv_sec - start.tv_sec;
  delta.tv_nsec = labs(finish.tv_nsec - start.tv_nsec);

  total_time = (delta.tv_sec) + (delta.tv_nsec)/(double)BILLION;
  per_move_time = (total_time) / size;
  per_move_time = per_move_time/iters;      //splitting division into two steps means we won't have overflow errors


  checksum = sum_array(xs, size) + sum_array(ys, size) + sum_array(zs, size);

  printf("Time per coordinate: %.17g\n", per_move_time);
  //printf("Here are the sums! %hhd", checksum);

  return 0;
}
