#include "main.hh"

  void Timer::operator() (double* answer){
    double time_total = 0.0;
    bool useArray2;
    int_t index;
    int_t oppIndex;
    int i;
    int j;
    int_t tmp;
    struct timespec start, finish, delta;

    random_device device;
    mt19937 generator(device());
    uniform_int_distribution<int_t> distr(1,length);
    uniform_int_distribution<int_t> distr2(1,secondArraySize);

    vector<int_t> array(length,distr(generator));
    vector<int_t> array2(secondArraySize,distr2(generator));
    if (secondArraySize > 0){
      useArray2 = true;
    } else{
      vector<int_t> array2;
      useArray2 = false;
    }


    for(i = 0; i < searches; i++){
      if (secondArraySize >0){
        //this for loop could probably be optimized
        for (j = 0; j < 50; j++){
          oppIndex = distr2(generator);
          tmp = array2[oppIndex];
        }
      }
      index = distr(generator);
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
    *answer = time_total;
  }

//int main(int argc, char *argv[]){
int main(){
  // PARAMETERS alter the assignment of thee variabls to change the program:
    int_t initialLength = 100;
    int_t maxLength = 10000000;
    int_t array2Length = 0;
    int_t prefetch = 0;
    double searches = 1000000;
    int_t numThreads = 1;

  double total_time = 0.0;
  int i;
  struct timespec start, end, delta;

/*
  if (argc != 6){
    printf("Required arguements: vector_length(N), number of searches (S), number of threads (T), second array size (S2), prefetch? (B)");
    return 0;
  }else {
    length = atoi(argv[1]);
    searches = atoi(argv[2]);
    numThreads = atoi(argv[3]);
    array2Length = atoi(argv[4]);
    prefetch = atoi(argv[5]);
    if ((length <= 0) || (searches <= 0) || (array2Length < 0) || !((prefetch == 0) || (prefetch == 1))){
      printf("Please make sure you used valid command line inputs, prefetch should only be 0 or 1 all other values positive");
      return 0;
    }
  }
*/
  //declare these vectors after so they can be initialized too
  double threshhold = 500;
  auto temp = numThreads - 250;
  if (temp >0){
    numThreads = 250;
  }
  vector<thread> threadVector;
  vector<double> answerVect(numThreads, 0);
  //vector<Timer> times(numThreads, Timer(initalLength,searches,array2Length,prefetch));

  clock_gettime(CLOCK_REALTIME, &start);
  for (i = 0; i < numThreads;i++){
    threadVector.emplace_back(times[i], &answerVect[i]);
  }
  for (i = 0; i < numThreads;i++){
    threadVector[i].join();
  }
  clock_gettime(CLOCK_REALTIME, &end);
  delta.tv_sec = end.tv_sec - start.tv_sec;
  delta.tv_nsec = end.tv_nsec - start.tv_nsec;
  double time = (delta.tv_sec) + (delta.tv_nsec)/(double)NANOS;

  double maxTime = 0;
  double minTime = answerVect[0];
  int_t doNotInclude = 0;
  for (i = 0; i < numThreads; i ++){
    if (answerVect[i] < threshhold){
      if (answerVect[i] > maxTime){
        maxTime = answerVect[i];
      }
      if (answerVect[i] < minTime){
        minTime = answerVect[i];
      }
      total_time += answerVect[i];
      //printf("Data from thread %d:  %f ns\n",i,answerVect[i]);
    }else{
      doNotInclude +=1;
    }
  }

  auto flag = temp;
  auto threadCount = 250;
  if (temp > 0){ numThreads +=temp;}
  while (flag > 0){
    if (flag > 250){
      flag -= 250;
      temp = 250;
    }else{
      temp = flag;
      flag = -1;
    }
    vector<thread> threadVector;
    vector<double> answerVect(temp, 0);
    vector<Timer> times(temp, Timer(length,searches,array2Length,prefetch));

    clock_gettime(CLOCK_REALTIME, &start);
    for (i = 0; i < temp;i++){
      threadVector.emplace_back(times[i], &answerVect[i]);
    }
    for (i = 0; i < temp;i++){
      threadVector[i].join();
    }
    clock_gettime(CLOCK_REALTIME, &end);
    delta.tv_sec = end.tv_sec - start.tv_sec;
    delta.tv_nsec = end.tv_nsec - start.tv_nsec;
    time += (delta.tv_sec) + (delta.tv_nsec)/(double)NANOS;


    total_time += answerVect[0];
    for (i = 1; i < temp; i ++){
      if (answerVect[i] < threshhold){
        if (answerVect[i]>maxTime){
          maxTime = answerVect[i];
        }
        if (answerVect[i] < minTime){
          minTime = answerVect[i];
        }
        total_time += answerVect[i];
        //printf("Data from thread %d:  %f ns\n",i+int(threadCount),answerVect[i]);
      }else{
        doNotInclude += 1;
      }
      }
    threadCount +=250;
    }

  printf("The average thread completion time was:  %f ns\n",total_time/(numThreads - doNotInclude));
  printf("The total threads over the threshhold time (of %f) was:  %lld\n", threshhold, doNotInclude);
  printf("The longest thread execution was:   %f ns\n", maxTime);
  printf("The shortest thread execution time was:  %f ns\n", minTime);
  printf("The total Completion time was:  %f s\n",time);
  return 0;
}
