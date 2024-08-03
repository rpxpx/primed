
#include <stdio.h>
#include "colorspace.h"


int main(void){

  FILE *fp;
  fp = fopen("cmap_legend.ppm","w");
  cmap_legend(fp, cmap_deepblue, 10, 16);
  fclose(fp);
  
  return 0;
}
