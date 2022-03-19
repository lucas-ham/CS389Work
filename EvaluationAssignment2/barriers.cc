#include "barriers.hh"
using namespace std;


void centralized(int_t numThreads, bool input_sense) {
  bool local_sense = !input_sense;
  if (atomic_fetch_add(&cent_counter,1) == numThreads) {
    cent_counter = 1;
    cent_sense = local_sense;
  } else {
    while (cent_sense != local_sense) {}
} }

void runCentralized(int_t numThreads, bool input_sense){
  int_t i;
  random_device device;
  mt19937 generator(device());
  uniform_int_distribution<int_t> distr(1,maxWait);
  uniform_int_distribution<int_t> loopDist(1,loops);
  int_t times = loopDist(generator);

  int_t wait;
  for (i=0;i<times;i++){
    wait = distr(generator);
    sleep(double(wait)/double(1000));
  }
  centralized(numThreads, input_sense);
}

void dissemination(int_t numThreads, int_t tid, bool* sense, int_t* parity) {
  int_t round;
  int_t partner;
  bool local_sense = *sense;
  for (round = 0; round < diss_rounds; round++) {
    partner = (tid + int(pow(2,round)))%(numThreads);
    assert(partner>=0);
    assert(partner<numThreads);
    diss_flags[partner][*parity][round] = !local_sense;
    while (diss_flags[tid][*parity][round] == local_sense){}
  }
  if (*parity == 1) {
    *sense = !local_sense;
  }
  *parity = 1 - *parity;

}

void runDissemination(int_t numThreads, int_t tid){
  bool sense = false;
  int_t parity = 0;
  int_t i;
  random_device device;
  mt19937 generator(device());
  uniform_int_distribution<int_t> distr(1,maxWait);
  uniform_int_distribution<int_t> loopDist(1,loops);
  int_t times = loopDist(generator);

  int_t wait;
  for (i=0;i<times;i++){
    wait = distr(generator);
    sleep(double(wait)/double(1000));
  }
  dissemination(numThreads, tid, &sense, &parity);
}

void MCS(int_t numThreads, int_t tid, bool* sense){
  int_t left_child = 2*tid + 1;
  int_t right_child = 2*(tid+1);
  if (right_child < numThreads){
    while ((mcs_flags[left_child][0] != *sense) || (mcs_flags[right_child][0] != *sense)){}
  } else if (left_child< numThreads){
    while (!(mcs_flags[left_child][0] != *sense)){}
  }
  mcs_flags[tid][0] = *sense;
  if (tid > 0){
    while (mcs_flags[(tid-1)/2][1] != *sense){}
  }
  mcs_flags[tid][1] = *sense;
  *sense = !*sense;
}


void runMCS(int_t numThreads, int_t tid){
  bool sense = true;
  int_t i;
  random_device device;
  mt19937 generator(device());
  uniform_int_distribution<int_t> distr(1,maxWait);
  uniform_int_distribution<int_t> loopDist(1,loops);
  int_t times = loopDist(generator);

  int_t wait;
  for (i=0;i<times;i++){
    wait = distr(generator);
    sleep(double(wait)/double(1000));
  }
  MCS(numThreads, tid, &sense);
}
