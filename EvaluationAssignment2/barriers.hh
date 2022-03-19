#pragma once

#include <cstdlib>
#include <atomic>
#include <iostream>
#include <vector>
#include <math.h>
#include <thread>
#include <random>
#include <functional>
#include <unistd.h>
#include <mutex>

#include <assert.h>

typedef int64_t int_t;
#define NANOS 1000000000


//PARAMETERS
int_t number_threads = 200;
int_t maxWait = 250; //1000 ms = 1 sec, divide by 1000 before input to sleep
int_t loops = 4; //


//Initialize some global scope variables needed by barriers
std::atomic<int_t> cent_counter = 1;
bool cent_sense = false;

int_t diss_rounds = ceil((log(number_threads))/(log(2)));
std::vector<std::vector<std::vector<bool>>> diss_flags(number_threads, std::vector<std::vector<bool>> (2, std::vector<bool> (diss_rounds, false)));

std::vector<std::vector<bool>> mcs_flags(number_threads, std::vector<bool>(2,false));



void centralized(int_t numThreads, bool input_sense = cent_sense);
void runCentralized(int_t numThreads, bool input_sense = cent_sense);

void dissemination(int_t numThreads, int_t tid, bool* sense, int_t* parity);
void runDissemination(int_t numThreads, int_t tid);

void MCS(int_t numThreads, int_t tid, bool* sense);
void runMCS(int_t numThreads, int_t tid);

/* tests to make sure that all threads exit together */
int_t test_barrier_holds(int_t numThreads, int_t barrier);

void wait_thread_Cent(int_t numThreads);
void overWrite_thread_Cent(int_t numThreads);

void wait_thread_Diss(int_t numThreads, int_t tid);
void overWrite_thread_Diss(int_t numThreads, int_t tid);

void wait_thread_MCS(int_t numThreads, int_t tid);
void overWrite_thread_MCS(int_t numThreads, int_t tid);


/* tests to ensure a barrier can be reused without issue*/
void run_mult_cent(int_t numThreads, int_t runs);
void iterate_cent(int_t numThreads, int_t runs);

void run_mult_diss(int_t numThreads, int_t runs, int_t tid);
void iterate_diss(int_t numThreads, int_t runs);

void run_mult_mcs(int_t numThreads, int_t runs, int_t tid);
void iterate_mcs(int_t numThreads, int_t runs);


/*combines the last two tests to ensure ordering even in reuse scenarios */
void test_cent_hold_mult(int_t numThreads, int_t runs);
void cent_mult_wait(int_t numThreads, bool input_sense = cent_sense);
void cent_mult_overWrite(int_t numThreads, bool input_sense = cent_sense);

void test_diss_hold_mult(int_t numThreads, int_t runs);
void diss_mult_wait(int_t numThreads, int_t tid, bool sense, int_t parity);
void diss_mult_overWrite(int_t numThreads, int_t tid, bool sense, int_t parity);
void more_inputs_diss(int_t numThreads, int_t tid, bool sense, int_t parity);

void test_mcs_hold_mult(int_t numThreads, int_t runs);
void mcs_mult_wait(int_t numThreads, int_t tid, bool sense);
void mcs_mult_overWrite(int_t numThreads, int_t tid, bool sense);
void more_inputs_mcs(int_t numThreads, int_t tid, bool sense);


/*each thread waits a random amount of time before entering barrier*/
void test_rand_wait_cent(int_t numThreads);
void cent_waiter(int_t numThreads);
void test_rand_wait_diss(int_t numThreads);
void diss_waiter(int_t numThreads,int_t tid);
void test_rand_wait_mcs(int_t numThreads);
void mcs_waiter(int_t numThreads, int_t tid);




/* Functions for Timing and Getting Graph Data */
void cent_timer(int_t numThreads, double* answer);

void diss_timer(int_t numThreads, int_t tid, double* answer);

void mcs_timer(int_t numThreads, int_t tid, double* answer);
