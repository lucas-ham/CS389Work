#include "timing.hh"

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
    uniform_int_distribution<int_t> distr(0,length-1);
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
        for (j = 0; j < 10; j++){
          oppIndex = distr2(generator);
          tmp = array2[oppIndex];
        }
      }
      index = distr(generator);
      if (prefetch){
        tmp = array[index];
      }
      clock_gettime(CLOCK_REALTIME, &start);
      tmp = array[index];
      clock_gettime(CLOCK_REALTIME, &finish);
      delta.tv_sec = finish.tv_sec - start.tv_sec;
      delta.tv_nsec = labs(finish.tv_nsec - start.tv_nsec);
      time_total += (delta.tv_sec) + (delta.tv_nsec)/(double)NANOS;
    }
    time_total = time_total/(double)searches;
    time_total = time_total*(double)NANOS;
    *answer = time_total;
  }
int_t calculatepower(int_t start, int_t end){
  return floor(log (end - start));
}

int main(){
  // PARAMETERS alter the assignment of thee variabls to change the program:
    int_t initialLength = 100;
    int_t maxLength = 1000000000;
    double searches = 10000000;

    int_t array2Length = 0;
    int_t prefetch = 0;

  int i;
  int_t numAccesses;
  //struct timespec start, end, delta;

  numAccesses = calculatepower(initialLength, maxLength);
  auto timer = Timer(initialLength, searches, array2Length, prefetch);
  vector<double> answerVect(numAccesses, 0);
  printf("Buffer Size (bytes),Completion Time (ns)\n");
  for (i=0;i<numAccesses;i++){
    for (i = 0; i < numAccesses ; i ++){
      timer(&answerVect[i]);
      printf("%lld,%f\n",initialLength*sizeof(int_t), answerVect[i]);
      initialLength *= 2;
      timer.updateLength(initialLength);
    }
  }
  return 0;

}
