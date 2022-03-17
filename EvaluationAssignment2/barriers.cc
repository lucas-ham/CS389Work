#include "barriers.hh"

void Centralized::operator() (int_t threadID) {
  bool local = ! sense;
  if (counter.fetch_add(1) == numThreads-1) {
    counter = 1;
    sense = local;
  }else{
    while (sense != local){}
  }
}


Dissemination::Dissemination(int_t num){
  numThreads = num;
  numRounds = floor((log (numThreads))/(log (2)));
  vector<vector<vector<bool>>> flag (numThreads, vector<vector<bool>> (2, vector<bool> (numRounds,false)));
  flags = flag;
  sense = false;
  parity = false;
}

void Dissemination::operator() (int_t threadID) {
  int_t partner;
  int_t round;
  for (round = 0; round <= numRounds; round++) {
    partner = (threadID + 2^(round))%numThreads;
    flags[partner][parity][round] = !sense;
    while (flags[threadID][parity][round] == sense){}
  }
  if (parity == true){ sense = !sense;}
  parity = !parity;
}


MCS::MCS(int_t num){
  numThreads = num;
}


void MCS::operator() (int_t threadID) {
  
}
