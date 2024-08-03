
#include <stdio.h>
#include "distributions.h"

#define RNDMZ 16

int main(int argc, char *argv[]){
  if (argc < 2){
    printf("err: supply integer at invocation\n");
    return 1;
  }
  
  ulong num;
  num = atol(argv[1]);
  

  int i;
  
  float *fs = gen_fracset(num, RNDMZ);
  
  for (i=0; i<num; i++)
    printf(" %f",*(fs+i));
  printf("\n");

  float *sfs = shrink_fracset(fs, num, num-1);
  
  for (i=0; i<num; i++)
    printf(" %f",*(sfs+i));
  printf("\n");
  
  return 0;
}
