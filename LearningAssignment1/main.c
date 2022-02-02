#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int main() {
  int seed;
  int listLength;
  int curr;
  double mean = 0;
  bool seedDone = false;
  bool listDone = false;
  int num;


  while (!seedDone){                                                      //check to make sure valid input
    printf("Please enter the seed for the random number generator:  ");
    num = scanf(" %d", &seed);
    if (num != 1){
      printf("\nPlease Try Again, integers only please\n");
      while(getchar() != '\n');                                 //ensures that the stream is empty for the next scanf to take data
    } else {
      seedDone = true;
    }
  }
  printf("\n\n");
  while(getchar() != '\n');                                   //clear stream between data entries

  while (!listDone) {                                           //ensure valid input
    printf("Please enter the total numbers to be added:  ");
    if (scanf(" %d", &listLength) != 1){
      printf("Please Try Again, integers only please\n");
      while(getchar() != '\n');                                 //clear data stream
    } else {
      listDone = true;
    }
  }

  srand(seed);
  for (int i = 0; i < listLength; i++) {
    curr = rand();
    mean += curr;
    printf("\n %d", curr);
  }

  printf("\n And the mean is %f", mean/listLength);

  return 0;
}
