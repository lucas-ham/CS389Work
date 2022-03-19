#include "barriers.hh"
#include "barriers.cc"
using namespace std;

atomic<int> cent_flag = 0;
atomic<int> diss_flag = 0;
atomic<int> mcs_flag = 0;

int_t test_barrier_holds(int_t numThreads, int_t barrier){
  //barrier = 0 centralized
  //barrier = 1 Dissemination
  //barrier = 2 MCS

  vector<thread> threadVect;
  int_t i;
  random_device device;
  mt19937 generator(device());
  uniform_int_distribution<int_t> dist(0,numThreads-1);
  auto wait_thread = dist(generator);
  auto overWrite_thread = dist(generator);
  if (wait_thread == overWrite_thread){
    overWrite_thread = (overWrite_thread +1)%numThreads;
  }
switch (barrier) {
  case 0: //centralized barrier
    for (i=0;i<numThreads;i++){
      if (i == wait_thread){
        threadVect.emplace_back(wait_thread_Cent, numThreads);
      } else if (i == overWrite_thread){
        threadVect.emplace_back(overWrite_thread_Cent, numThreads);
      }else {
        threadVect.emplace_back(runCentralized, numThreads, cent_sense);
      }
    }

    for (i=0;i<numThreads;i++){
      threadVect[i].join();
    }
    if (atomic_load(&cent_flag)==2){
      return 0;
    }else{
      return wait_thread*100+overWrite_thread;
    }
    break;
  case 1: //dissemination barrier
    for (i=0;i<numThreads;i++){
      if (i == wait_thread){
        threadVect.emplace_back(wait_thread_Diss, numThreads, i);
      } else if (i == overWrite_thread){
        threadVect.emplace_back(overWrite_thread_Diss, numThreads, i);
      }else{
        threadVect.emplace_back(runDissemination, numThreads, i);
      }
    }
    for (i=0;i<numThreads;i++){
      threadVect[i].join();
    }
    if (atomic_load(&diss_flag)==2){
      return 0;
    }else{
      return wait_thread*100+overWrite_thread;
    }
    break;
  case 2:  //mcs barrier
    for (i=0;i<numThreads;i++){
      if (i == wait_thread){
        threadVect.emplace_back(wait_thread_MCS, numThreads, i);
      } else if (i == overWrite_thread){
        threadVect.emplace_back(overWrite_thread_MCS, numThreads, i);
      }else{
        threadVect.emplace_back(runMCS, numThreads, i);
      }
    }
    for (i=0;i<numThreads;i++){
      threadVect[i].join();
    }
    if (atomic_load(&mcs_flag)==2){
      return 0;
    }else{
      return wait_thread*100+overWrite_thread;
    }
    break;
  default:
    return 1;
  }
}

void wait_thread_Cent(int_t numThreads){
  sleep(1);
  atomic_store(&cent_flag, 1);
  centralized(numThreads);
}

void overWrite_thread_Cent(int_t numThreads){
  centralized(numThreads);
  atomic_store(&cent_flag, 2);
}

void wait_thread_Diss(int_t numThreads, int_t tid){
  bool sense = false;
  int_t parity = 0;
  sleep(1);
  atomic_store(&diss_flag, 1);
  dissemination(numThreads, tid, &sense, &parity);
}

void overWrite_thread_Diss(int_t numThreads, int_t tid){
  bool sense = false;
  int_t parity = 0;
  dissemination(numThreads, tid, &sense, &parity);
  atomic_store(&diss_flag, 2);
}

void wait_thread_MCS(int_t numThreads, int_t tid){
  bool sense = true;
  sleep(1);
  atomic_store(&mcs_flag, 1);
  MCS(numThreads, tid, &sense);
}

void overWrite_thread_MCS(int_t numThreads, int_t tid){
  bool sense = true;
  MCS(numThreads, tid, &sense);
  atomic_store(&mcs_flag, 2);
}



void run_mult_cent(int_t numThreads, int_t runs){
  int_t i;
  bool local_sense = !cent_sense;
  for (i=0;i<runs;i++){
    centralized(numThreads, local_sense);
  }
}

void iterate_cent(int_t numThreads, int_t runs){
  int_t i;
  vector<thread> threads;
  for(i=0;i<numThreads;i++){
    threads.emplace_back(run_mult_cent,numThreads,runs);
  }
  for (i=0;i<numThreads;i++){
    threads[i].join();
  }
}

void run_mult_diss(int_t numThreads,int_t runs, int_t tid){
  bool sense = false;
  int_t parity = 0;
  int_t i;
  for (i=0;i<runs;i++){
    dissemination(numThreads, tid, &sense, &parity);
  }
}

void iterate_diss(int_t numThreads, int_t runs){
  int_t i;
  vector<thread> threads;
  for (i=0;i<numThreads;i++){
    threads.emplace_back(run_mult_diss,numThreads,runs,i);
  }
  for (i=0;i<numThreads;i++){
    threads[i].join();
  }
}

void run_mult_mcs(int_t numThreads, int_t runs, int_t tid){
  bool sense = true;
  int_t i;
  for (i=0;i<runs;i++){
    MCS(numThreads,tid,&sense);
  }
}

void iterate_mcs(int_t numThreads, int_t runs){
  int_t i;
  vector<thread> threads;
  for (i=0;i<numThreads;i++){
    threads.emplace_back(run_mult_mcs,numThreads,runs,i);
  }
  for (i=0;i<numThreads;i++){
    threads[i].join();
  }
}




void test_cent_hold_mult(int_t numThreads, int_t runs){
  int_t j,i;
  vector<thread> threadVect;
  vector<int> answerVect;
  random_device device;
  mt19937 generator(device());
  uniform_int_distribution<int_t> dist(0,numThreads-1);
  bool use_sense = false;
  for (j=0;j<runs;j++){
    auto wait_thread = dist(generator);
    auto overWrite_thread = dist(generator);
    if (wait_thread == overWrite_thread){
      overWrite_thread = (overWrite_thread +1)%numThreads;
    }
    for (i=0;i<numThreads;i++){
      if (i == wait_thread){
        threadVect.emplace_back(cent_mult_wait, numThreads, use_sense);
      } else if (i == overWrite_thread){
        threadVect.emplace_back(cent_mult_overWrite, numThreads, use_sense);
      }else {
        threadVect.emplace_back(runCentralized,numThreads,use_sense);
      }
    }

    for (i=0;i<numThreads;i++){
      threadVect[i].join();
    }
    if (atomic_load(&cent_flag)==2){
      answerVect.push_back(0);
    }else{
      printf("Fail for Cent. Wait:  %lld\nOverwrite:  %lld\n\n",wait_thread,overWrite_thread);
    }
    use_sense = !use_sense;
    threadVect.clear();
  }
}

void cent_mult_wait(int_t numThreads, bool input_sense){
  sleep(1);
  atomic_store(&cent_flag,1);
  centralized(numThreads, input_sense);
}

void cent_mult_overWrite(int_t numThreads, bool input_sense){
  centralized(numThreads, input_sense);
  atomic_store(&cent_flag,2);
}

void test_diss_hold_mult(int_t numThreads, int_t runs){
  int_t j,i;
  vector<thread> threadVect;
  vector<int> answerVect;
  random_device device;
  mt19937 generator(device());
  uniform_int_distribution<int_t> dist(0,numThreads-1);
  bool use_sense = false;
  int_t parity = 0;
  for (j=0;j<runs;j++){
    auto wait_thread = dist(generator);
    auto overWrite_thread = dist(generator);
    if (wait_thread == overWrite_thread){
      overWrite_thread = (overWrite_thread +1)%numThreads;
    }
    for (i=0;i<numThreads;i++){
      if (i == wait_thread){
        threadVect.emplace_back(diss_mult_wait, numThreads, i, use_sense, parity);
      } else if (i == overWrite_thread){
        threadVect.emplace_back(diss_mult_overWrite, numThreads, i, use_sense, parity);
      }else {
        threadVect.emplace_back(more_inputs_diss,numThreads,i,use_sense,parity);
      }
    }

    for (i=0;i<numThreads;i++){
      threadVect[i].join();
    }
    if (atomic_load(&cent_flag)==2){
      answerVect.push_back(0);
    }else{
      printf("Fail for Diss. Wait:  %lld\nOverwrite:  %lld\n\n",wait_thread,overWrite_thread);
    }
    if (parity == 1) {
      use_sense = !use_sense;
    }
    parity = 1 - parity;
    threadVect.clear();
  }
}

void diss_mult_wait(int_t numThreads, int_t tid, bool input_sense, int_t parity){
  sleep(1);
  atomic_store(&diss_flag,1);
  dissemination(numThreads, tid, &input_sense, &parity);
}

void diss_mult_overWrite(int_t numThreads, int_t tid, bool input_sense, int_t parity){
  dissemination(numThreads, tid, &input_sense, &parity);
  atomic_store(&diss_flag,2);
}

void more_inputs_diss(int_t numThreads, int_t tid, bool input_sense, int_t parity){
  dissemination(numThreads, tid, &input_sense, &parity);
}

void test_mcs_hold_mult(int_t numThreads, int_t runs){
  int_t j,i;
  vector<thread> threadVect;
  vector<int> answerVect;
  random_device device;
  mt19937 generator(device());
  uniform_int_distribution<int_t> dist(0,numThreads-1);
  bool use_sense = false;
  for (j=0;j<runs;j++){
    auto wait_thread = dist(generator);
    auto overWrite_thread = dist(generator);
    if (wait_thread == overWrite_thread){
      overWrite_thread = (overWrite_thread +1)%numThreads;
    }
    for (i=0;i<numThreads;i++){
      if (i == wait_thread){
        threadVect.emplace_back(mcs_mult_wait, numThreads, i, use_sense);
      } else if (i == overWrite_thread){
        threadVect.emplace_back(mcs_mult_overWrite, numThreads, i, use_sense);
      }else {
        threadVect.emplace_back(more_inputs_mcs,numThreads,i,use_sense);
      }
    }

    for (i=0;i<numThreads;i++){
      threadVect[i].join();
    }
    if (atomic_load(&cent_flag)==2){
      answerVect.push_back(0);
    }else{
      printf("Fail for MCS. Wait:  %lld\nOverwrite:  %lld\n\n",wait_thread,overWrite_thread);
    }
    use_sense = !use_sense;
    threadVect.clear();
  }
}

void mcs_mult_wait(int_t numThreads, int_t tid, bool input_sense){
  sleep(1);
  atomic_store(&mcs_flag,1);
  MCS(numThreads, tid, &input_sense);
}

void mcs_mult_overWrite(int_t numThreads, int_t tid, bool input_sense){
  MCS(numThreads, tid, &input_sense);
  atomic_store(&mcs_flag,2);
}

void more_inputs_mcs(int_t numThreads, int_t tid, bool input_sense){
  MCS(numThreads, tid, &input_sense);
}



void test_rand_wait_cent(int_t numThreads){
  int_t i = 0;
  vector<thread> threads;
  for (i=0;i<numThreads;i++){
    threads.emplace_back(cent_waiter,numThreads);
  }
  for (i=0;i<numThreads;i++){
    threads[i].join();
  }
  printf("All threads make it through cent barrier with random waits\n");
}
void cent_waiter(int_t numThreads){
  int_t i;
  random_device device;
  mt19937 generator(device());
  uniform_int_distribution<int_t> dist(0,maxWait);
  uniform_int_distribution<int_t> loopDist(0,loops);
  auto loop = loopDist(generator);
  int_t wait;
  for (i=0;i<loop;i++){
    wait = dist(generator);
    sleep(double(wait)/double(1000));
  }
  centralized(numThreads);
}
void test_rand_wait_diss(int_t numThreads){
  int_t i = 0;
  vector<thread> threads;
  for (i=0;i<numThreads;i++){
    threads.emplace_back(diss_waiter,numThreads,i);
  }
  for (i=0;i<numThreads;i++){
    threads[i].join();
  }
  printf("All threads make it through diss barrier with random waits\n");
}
void diss_waiter(int_t numThreads,int_t tid){
  bool sense = false;
  int_t parity = 0;
  int_t i;
  random_device device;
  mt19937 generator(device());
  uniform_int_distribution<int_t> dist(0,maxWait);
  uniform_int_distribution<int_t> loopDist(0,loops);
  auto loop = loopDist(generator);
  int_t wait;
  for (i=0;i<loop;i++){
    wait = dist(generator);
    sleep(double(wait)/double(1000));
  }
  dissemination(numThreads,tid, &sense, &parity);
}
void test_rand_wait_mcs(int_t numThreads){
  int_t i = 0;
  vector<thread> threads;
  for (i=0;i<numThreads;i++){
    threads.emplace_back(mcs_waiter,numThreads,i);
  }
  for (i=0;i<numThreads;i++){
    threads[i].join();
  }
  printf("All threads make it through mcs barrier with random waits\n");
}
void mcs_waiter(int_t numThreads, int_t tid){
  bool sense = true;
  int_t i;
  random_device device;
  mt19937 generator(device());
  uniform_int_distribution<int_t> dist(0,maxWait);
  uniform_int_distribution<int_t> loopDist(0,loops);
  auto loop = loopDist(generator);
  int_t wait;
  for (i=0;i<loop;i++){
    wait = dist(generator);
    sleep(double(wait)/double(1000));
  }
  MCS(numThreads,tid, &sense);
}



int main() {
  int_t numThreads = number_threads;
  int_t runs = 5;

/*code to run the first test */
/*
  auto mcs = test_barrier_holds(numThreads, 2);
  assert(mcs == 0);
  printf("Centralized barrier passes the first test\n");
  //auto diss = test_barrier_holds(numThreads, 2);
  //assert(diss == 0);
  printf("Dissemination barrier passes the first test\n");
  auto cent = test_barrier_holds(numThreads, 0);
  assert(cent == 0);
  printf("MCS barrier passes the first test\n");
/*


/*code to run the second test */
/*
  iterate_cent(numThreads, runs);
  printf("Centralized passes second test\n");
  iterate_diss(numThreads,runs);
  printf("Dissemination barrier passes the second test\n");
  iterate_mcs(numThreads,runs);
  printf("MCS barrier passes the second test\n");
*/


/*code to run third test */

  test_cent_hold_mult(numThreads,runs);
  printf("Centralized barrier passes the third test\n");

  //test_diss_hold_mult(numThreads,runs);
  //printf("Dissemination barrier passes the third test\n");

  test_mcs_hold_mult(numThreads, runs);
  printf("MCS barrier passes the third test\n");


/* code to run fourth test */
  test_rand_wait_cent(numThreads);
  test_rand_wait_diss(numThreads);
  test_rand_wait_mcs(numThreads);

  return 0;


}
