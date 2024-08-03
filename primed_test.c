
#include <stdio.h>
#include <math.h>
#include "primed.h"

#define COMP_PARAM 5

int main(int argc, char *argv[]){
  if (argc < 2){
    printf("err: supply integer at invocation\n");
    return 1;
  }
  
  ulong num, sqr;
  num = atol(argv[1]);
  sqr = ceil(sqrt(num));

  printf("\n* prime-counting Gaussian approx on %ld: %ld\n", num, pi_approx_Gauss(num));
  printf("\n");
  
  printf("* prime-counting on %ld: ", num); fflush(stdout);
  printf("%ld\n", pi(num));
  printf("\n");

  printf("* compiling prime sequence up to %ld:\n", num);
  inta prms = primes(num);
  /* print last 10 elements only */
  print_inta_slice(prms, prms.l-10, prms.l);
  wipe_inta(&prms);  
  printf("\n");
  
  printf("* prime decomposition of %ld: ",num);
  inta decomp = decompose(num);
  print_inta(decomp);
  wipe_inta(&decomp);
  printf("\n");

  printf("* divizors of %ld:\n",num);
  inta divs = divizors(num);
  print_inta(divs);
  sort_inta(divs);
  printf("...quicksort, Hoare partitioned:\n");
  print_inta(divs);  
  wipe_inta(&divs);  
  printf("\n");
  
  printf("* Example 10-square (1...100) ascii Ulam spiral. Primes coded 1.\n");  
  ulam_spiral_ascii(10, 1);
  printf("\n");
  
  printf("* writing out %ld-square (1...%ld) standard Ulam spiral to ulam.pbm. Primes coded white.\n",sqr,sqr*sqr);
  FILE *fp;
  fp = fopen("ulam.pbm","w");
  ulam_spiral(fp, sqr, 1);
  fclose(fp); 
  printf("\n");

  printf("* compiling compositionality values to %ld:\n",num);
  inta comps = compositionality(num);
  /* print last 40 elements only */
  print_inta_slice(comps, comps.l-40, comps.l);
  wipe_inta(&comps);  
  printf("\n");

  printf("* Example 10-square (1...100) ascii enhanced composite Ulam spiral.\n");
  ulam_spiral_composites_ascii(10);
  printf("\n");

  printf("* writing out %ld-square (1...%ld) enhanced composite Ulam spiral to ulam_c.ppm.\n",sqr,sqr*sqr);
  printf("* writing out color legend to ulam_c_legend.ppm\n");
  FILE *lfp;
  fp = fopen("ulam_c.ppm","w");
  lfp = fopen("ulam_c_legend.ppm","w");
  ulam_spiral_composites(fp, cmap_heat_, 0, sqr, COMP_PARAM, lfp, 6);
  fclose(fp);
  fclose(lfp);
  
  return 0;
}
