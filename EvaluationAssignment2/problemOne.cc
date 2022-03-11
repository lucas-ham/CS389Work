#include "main.hh"


int main(int argc, char *argv[]){
  int_t length;
  int_t* array;
  int_t searches;

  int_t prefetch;

  int i;
  clock_t start, end;
  int_t tmp;
  int_t index;
  auto totTime = 0;


  if (argc != 4){
    printf("Required arguements: vector_length(N), number of searches (S), prefetch (B)");
    return 0;
  }else {
    length = atoi(argv[1]);
    searches = atoi(argv[2]);
    prefetch = atoi(argv[3]);
  }

  array = (int_t*)malloc(length*sizeof(int_t));

  srand(clock());

  int_t srt;

  for(i = 0; i < searches; i++){
    index = rand()%length;
    if (prefetch){
      srt = array[index];
    }
    start = clock();
    tmp = array[index];
    end = clock();
    totTime += end-start;

  }
  int_t s = searches/100;
  totTime = (totTime/s);
  printf("%f", ((float)totTime)/CLOCKS_PER_SEC);

  return 0;
}
