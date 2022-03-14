#include "main.hh"


double timer(int_t length, double searches, int_t secondArraySize, int_t prefetch){
  double time_total = 0.0;
  int_t* array;
  int_t* array2;
  bool useArray2;
  int_t index;
  int_t oppIndex;
  int i;
  int j;
  int_t tmp;
  struct timespec start, finish, delta;

  array = (int_t*)malloc(length*sizeof(int_t));
  if (secondArraySize > 0){
    array2 = (int_t*)malloc(secondArraySize*sizeof(int_t));
    useArray2 = true;
  } else{
    useArray2 = false;
  }
  srand(clock());
  for(i = 0; i < searches; i++){
    index = rand()%length;
    //in an attempt to make sure there's no useful prefetching going on we have a second array that we read from first to discourage prefetching
      //bunch of random calls to the array before hand to try and evict any elements from array
    if (useArray2){
      for (j = 0; j < 50; j++){
        oppIndex = rand()%secondArraySize;
        tmp = array2[oppIndex];
      }
    }

    if (prefetch){
      tmp = array[index];
    }
    clock_gettime(CLOCK_REALTIME, &start);
    //tmp = array[index];
    clock_gettime(CLOCK_REALTIME, &finish);

    delta.tv_sec = finish.tv_sec - start.tv_sec;
    delta.tv_nsec = labs(finish.tv_nsec - start.tv_nsec);

    time_total += (delta.tv_sec) + (delta.tv_nsec)/(double)NANOS;

  }
  time_total = time_total/(double)searches;
  time_total = time_total*(double)NANOS;
  return time_total;
}


int main(int argc, char *argv[]){
  //according to a google search:
    //l1 for my computer is 128 kb
    //l2 for my computer is 24 mb
  //int64_t has size 8 bytes, so length > 128000 can't be stored in l1 alone
    //need length > 2.4e+7 + 128000 to not be held in l2 either

  //according to system information, processor runs at 3.2 GHz
    //so there are 3.2 billion clock cycles/second
    //so roughly each clock cycle should take 0.32 nanoseconds on my computer

  int_t length;
  int_t array2Length;
  int_t prefetch;
  double total_time;
  double searches;


  if (argc != 5){
    printf("Required arguements: vector_length(N), number of searches (S), second array size (S2), prefetch? (B)");
    return 0;
  }else {
    length = atoi(argv[1]);
    searches = atoi(argv[2]);
    array2Length = atoi(argv[3]);
    prefetch = atoi(argv[4]);
    if ((length <= 0) || (searches <= 0) || (array2Length < 0) || !((prefetch == 0) || (prefetch == 1))){
      printf("Please make sure you used valid command line inputs, prefetch should only be 0 or 1 all other values positive");
      return 0;
    }
  }
  total_time = timer(length, searches, array2Length, prefetch);
  printf("%f",total_time);

  return 0;
}
