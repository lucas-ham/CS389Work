#include "main.hh"


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
  int_t* array;
  int_t* array1;
  double searches;
  int_t array2Length;
  int_t prefetch;
  bool useArray2;

  int i;
  int j;

  struct timespec strt, finish, delta;
  double total_time = 0.0;

  int_t tmp;
  int_t srt;
  int_t index;
  int_t oppIndex;


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

  array = (int_t*)malloc(length*sizeof(int_t));
  if (array2Length > 0){
    array1 = (int_t*)malloc(array2Length*sizeof(int_t));
    useArray2 = true;
  } else{
    useArray2 = false;
  }
  srand(clock());

  for (i = 0; i<length;i++){
    array[i] = rand();
  }
  if (useArray2){
    for (i = 0; i < array2Length;i++){
      array1[i] = rand();
    }
  }


  for(i = 0; i < searches; i++){
    index = rand()%length;
    //in an attempt to make sure there's no useful prefetching going on we have a second array that we read from first to discourage prefetching
      //bunch of random calls to the array before hand to try and evict any elements from array
    if (useArray2){
      for (j = 0; j < 50; j++){
        oppIndex = rand()%array2Length;
        srt = array1[oppIndex];
      }
    }

    if (prefetch){
      srt = array[index];
    }
    clock_gettime(CLOCK_REALTIME, &strt);
    tmp = array[index];
    clock_gettime(CLOCK_REALTIME, &finish);


  //per 1000 iterations, single iterations don't show up

  //CLOCKS_PER_SEC DOES NOT ACTUALLY MEAN THAT FOR A COMPUTER, JUST A GENERIC VALUE
      //
  indvTime = totTime/(searches/1);
  timeTmp = indvTime/(double)CLOCKS_PER_SEC;
  timeTmp = timeTmp*NANOS;

  printf("Number of clock ticks per thousand reads:  %f\n", indvTime);
  printf("Time (ns) per thousand reads:  %f\n", timeTmp);
    delta.tv_sec = finish.tv_sec - strt.tv_sec;
    delta.tv_nsec = labs(finish.tv_nsec - strt.tv_nsec);

  auto t = NANOS/(double)CLOCKS_PER_SEC;
  printf("%f\n",t);
    total_time += (delta.tv_sec) + (delta.tv_nsec)/(double)NANOS;

  /*
  messing around tring to find a better way to clock_t
    doesn't really seem like clock() is the right thing, but std::chrono::high_resolution_clock also isn'typedef isnt
    high_resolution_clock is better though, only has a few (order 100) nanoseconds of overhead, clock() isn't even really that precise to know
  auto s = std::chrono::high_resolution_clock::now();
  auto st = clock();
  for (i = 0; i<100;i++){;}
  auto en = clock();
  auto e = std::chrono::high_resolution_clock::now();

  std::cout << "It took me " << std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count() << " nanoseconds.\n";

  auto conv = (en-st)/(double)CLOCKS_PER_SEC;
  conv = conv*(double)NANOS;
  printf("%f\n", conv);
  */

  return 0;
}
