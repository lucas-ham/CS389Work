#include "barriers.hh"
#include "barriers.cc"
using namespace std;

void cent_timer(int_t numThreads, double* answer){
  struct timespec start, finish, delta;
  double time_total;
  clock_gettime(CLOCK_REALTIME, &start);
  runCentralized(numThreads);
  clock_gettime(CLOCK_REALTIME, &finish);
  delta.tv_sec = finish.tv_sec - start.tv_sec;
  delta.tv_nsec = labs(finish.tv_nsec - start.tv_nsec);
  time_total = (delta.tv_sec) + (delta.tv_nsec)/(double)NANOS;
  *answer = time_total;
}

void diss_timer(int_t numThreads, int_t tid, double* answer){
  struct timespec start, finish, delta;
  double time_total;
  clock_gettime(CLOCK_REALTIME, &start);
  runDissemination(numThreads, tid);
  clock_gettime(CLOCK_REALTIME, &finish);
  delta.tv_sec = finish.tv_sec - start.tv_sec;
  delta.tv_nsec = labs(finish.tv_nsec - start.tv_nsec);
  time_total = (delta.tv_sec) + (delta.tv_nsec)/(double)NANOS;
  *answer = time_total;
}


void mcs_timer(int_t numThreads, int_t tid, double* answer){
  struct timespec start, finish, delta;
  double time_total;
  clock_gettime(CLOCK_REALTIME, &start);
  runMCS(numThreads, tid);
  clock_gettime(CLOCK_REALTIME, &finish);
  delta.tv_sec = finish.tv_sec - start.tv_sec;
  delta.tv_nsec = labs(finish.tv_nsec - start.tv_nsec);
  time_total = (delta.tv_sec) + (delta.tv_nsec)/(double)NANOS;
  *answer = time_total;
}

double get_cent_time(int_t numThreads){
  int_t i;
  vector<thread> threads;
  double total_time = 0.0;
  vector<double> answerVect (numThreads, 0);
  for (i=0;i<numThreads;i++){
    threads.emplace_back(cent_timer,numThreads,&answerVect[i]);
  }
  for (i=0;i<numThreads;i++){
    threads[i].join();
    total_time += answerVect[i];
  }
  return total_time/(double)numThreads;
}

double get_diss_time(int_t numThreads){
  int_t i;
  vector<thread> threads;
  double total_time = 0.0;
  vector<double> answerVect (numThreads, 0);
  for (i=0;i<numThreads;i++){
    threads.emplace_back(diss_timer, numThreads, i, &answerVect[i]);
  }
  for (i=0;i<numThreads;i++){
    threads[i].join();
    total_time += answerVect[i];
  }
  return total_time/(double)numThreads;
}

double get_mcs_time(int_t numThreads){
  int_t i;
  vector<thread> threads;
  double total_time = 0.0;
  vector<double> answerVect (numThreads, 0);
  for (i=0;i<numThreads;i++){
    threads.emplace_back(diss_timer,numThreads,i,&answerVect[i]);
  }
  for (i=0;i<numThreads;i++){
    threads[i].join();
    total_time += answerVect[i];
  }
  return total_time/(double)numThreads;
}

void change_num_threads(int_t newThreads){
  number_threads = newThreads;
  int_t diss_rounds = ceil((log(number_threads))/(log(2)));
  std::vector<std::vector<std::vector<bool>>> diss_flags(number_threads, std::vector<std::vector<bool>> (2, std::vector<bool> (diss_rounds, false)));
  std::vector<std::vector<bool>> mcs_flags(number_threads, std::vector<bool>(2,false));
}

int main() {
  int_t numThreads = number_threads;
  int_t i;
  printf("Threads,CentTimes,DisseminationTimes,MCSTimes\n");
  for (i=4;i<=numThreads;i+=4){
    //printf("\nFor %lld Threads:  \n\n",i);
    change_num_threads(i);
    printf("%lld,",i);
    double cent_time = get_cent_time(i);
    //printf("Average time for Centralized:  %f s\n",cent_time);
    printf("%f,",cent_time);

    double diss_time = get_diss_time(i);
    //printf("Average time for Dissemination:  %f s\n",diss_time);
    printf("%f,",diss_time);

    double mcs_time = get_mcs_time(i);
    //printf("Average time for MCS:  %f s\n",mcs_time);
    printf("%f\n",mcs_time);
  }


}
