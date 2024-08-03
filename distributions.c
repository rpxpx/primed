
#include "distributions.h"


uint digit_count_int(int a){
  uint c = 1;
  while (a/=10)
    c++;
  return c;
}

ulong *prep_index(ulong l){
  ulong *ind = (ulong *)malloc(sizeof(ulong)*(l+1));  
  /* Two termini. The first is mobile and marks the end of meaningful data - loaded indices. */
  *(ind+0) = UL_END;
  *(ind+l) = UL_END;
  return ind;
}

/* Print to first terminus.*/
void print_index(ulong *ind){
  ulong i;
  for (i=0; *(ind+i)!=UL_END; i++)
    printf("%ld ", *(ind+i));
  printf("\n");
}

/* Print to second terminus: loaded indices and garbage in allocated mem. */
void print_index_full(ulong *ind){
  ulong i;
  for (i=0; *(ind+i)!=UL_END; i++)
    printf("%ld ", *(ind+i));
  printf("E ");
  for (i++; *(ind+i)!=UL_END; i++)
    printf("%ld ", *(ind+i));  
  printf("\n");  
}

bool is_in_index(ulong *ind, ulong n){
  ulong i;
  bool indexed = false;
  for (i=0; *(ind+i)!=UL_END && !indexed; i++)
    (*(ind+i)==n) && (indexed = true);
  return indexed;
}


float *fractionize_int(int *a, ulong l, int b){
  float *o = (float *)malloc(sizeof(float)*l);
  while (l--)
    *(o+l) = (b)?(*(a+l)/(float)b):0;
  return o;
}



/* Let a fracset be an array of floats summing to 1. */
/* Generate a fracset of length l.
 * Makes r random disruptions to an initially uniform assignment of 1/l.
 * A disruption deducts half from one element and adds it to another. */
float *gen_fracset(ulong l, uint r){
  float *f = NULL;
  if (l==0)
    return f;
  f = (float *)malloc(sizeof(float)*l);
  float init = 1.0/l;
  ulong i;
  for (i=0; i<l; i++)
    *(f+i) = init;
  
  ulong a, b;
  srand((unsigned)time(NULL));
  while (r--){
    a = rand()%l;
    b = rand()%l;
    *(f+a) /= 2;
    *(f+b) += *(f+a);
  }

  /* Last pass. Deduct from first element any residue >1 due to rounding.*/
  float sum = 0;
  while (l--)
    sum += *(f+l);
  *f -= (sum-1.0); 
  return f;
}


/* Submit an array of floats, which is not required to sum to 1.
 * Recalculate values in situ as fractions of the sum. */
int recalc_fracset(float *s, ulong l){
  if (l<2)
    return 1;
  
  /* First pass: take sum */
  float sum = sum_float(s, l);

  /* A fracset must sum to >0. */
  /* Check this to (arbitrarily chosen, yes) fixed epsilon of 0.001 */
  if (fabs(sum) < 0.001)
    return 1;
  
  /* Second pass: recalculate fracs */
  while (l--)
    *(s+l) = *(s+l)/sum;
  return 0;
}


/* Shrink a fracset from length l to ls.
 * Set smallest (l-ls) elements to zero, and recalculate fractions.
 * Size of set remains l */
float *shrink_fracset(float *s, ulong l, ulong ls){
  float *o = NULL;
  if (l<2)
    return o;

  /* error correction, in case ls > l */
  ls = MIN(ls, l);
  
  /* First pass. Check that numbers are in correct range, sum to 1, and find smallest element. */
  float f = *s;
  float sum = 0;
  ulong i;
  for (i=0; i<l; i++){
    if (*(s+i) < 0 || *(s+i) > 1)
      return o;
    else
      sum += *(s+i);
    if (*(s+i) < f)
      f = *(s+i);
  }
  
  /* A fracset must sum to 1. */
  /* Check this to (arbitrarily chosen) fixed epsilon of 0.001 */
  if (fabs(sum-1) > 0.001)
    return o;

  /* Copy fracset */
  o = (float *)malloc(sizeof(float)*l);  
  for (i=0; i<l; i++)
    *(o+i) = *(s+i);  
  
  /* Find (l-ls) smallest elements. */
  ulong *ind = mins_float(s, l, (l-ls));
  
  /* Sum the values of those elements and set to zero in out. */
  sum = 0;
  for (i=0; *(ind+i)!=UL_END; i++){
    sum += *(s+*(ind+i));
    *(o+*(ind+i)) = 0;
  }
  
  /* Recalculate fractions */
  sum = 1-sum;
  for(i=0; i<l; i++)
    *(o+i) /= sum;
  
  return o;
}




void *indx_int(void *a, ulong i){
  return (void *)((int *)a+i);
}

void *indx_float(void *a, ulong i){
  return (void *)((float *)a+i);
}

int cmp_int(void *n, void *m){
  if (*(int *)n < *(int *)m)
    return -1;  
  if (*(int *)n > *(int *)m)
    return 1;
  return 0;
}	     

int cmp_float(void *n, void *m){
  if (*(float *)n < *(float *)m)
    return -1;  
  if (*(float *)n > *(float *)m)
    return 1;
  return 0;
}



ulong min_indx(void *(*indx)(void *, ulong), int (*cmp)(void *, void *), void *a, ulong l){
  void *min = (*indx)(a, 0);
  ulong min_i = 0;
  ulong i;
  for (i=0; i<l; i++)
    if ((*cmp)((*indx)(a, i), min)==-1){ /* If i'th element < min */
      min = (*indx)(a, i);
      min_i = i;
    }      
  return min_i;
}

ulong *mins_indx(void *(*indx)(void *, ulong), int (*cmp)(void *, void *), void *a, ulong l, ulong n){
  ulong *minsi;  /* index of mins */
  if (l==0 || n==0){
    minsi = prep_index(0);
    return minsi;
  }  
  if (n>l)
    (n=l);  
  
  minsi = prep_index(n);
  
  /* First pass: find index of smallest element. */
  *(minsi+0) = min_indx(indx, cmp, a, l);
  *(minsi+1) = UL_END;
  
  /* Successive (n-1) passes */
  void *min;
  ulong i, min_i;
  ulong minsi_i;  
  for (minsi_i=1; minsi_i<n; minsi_i++){
    /* Find first element not yet indexed for initial min */
    for (i=0; i<l; i++){
      if (!is_in_index(minsi, i)){
	min = (*indx)(a, i);
	min_i = i;
	break;	
      }
    }
    /* ...continue scanning fwd, searching for smallest unindexed element < min */
    for (; i<l; i++){
      if ( (*cmp)((*indx)(a, i), min)==-1 && !is_in_index(minsi, i) ){	
	min = (*indx)(a, i);
	min_i = i;
	}
      }    
    /* add index of this to indices array & advance terminus */
    *(minsi+minsi_i) = min_i;
    *(minsi+minsi_i+1) = UL_END;    
  }  
  return minsi;      
}


ulong *mins_int(int *a, ulong l, ulong n){
  return mins_indx(indx_int, cmp_int, a, l, n);
}

long sum_int(int *a, ulong l){
  long s = 0;
  while (l--)
    s += *(a+l);
  return s;  
}

float mean_int(int *a, ulong l){
  return (float)sum_int(a, l)/l;
}

/* mutatively add n to all elements in a */
void add_int(int *a, ulong l, int n){  
  while (l--)
    *(a+l) += n;
  return;
}


ulong *mins_float(float *a, ulong l, ulong n){
  return mins_indx(indx_float, cmp_float, a, l, n);
}

float sum_float(float *a, ulong l){
  float s = 0;
  while (l--)
    s += *(a+l);
  return s;  
}


/* Squash a discrete curve down with push [0,1] into a more uniform distribution, returning a new array.*/
/* push must be in interval [0,1]: is the fraction of units above the mean that are transferred to elements below.*/
int *uniformize_int(int *a, ulong l, float push){
  if (l==0 || push < 0 || push > 1)
    return a;  
  
  int *o = (int *)malloc(sizeof(int)*l);

  /* Dismiss trivial case */
  if (l==1){
    *o = *a;
    return o;
  }
  
  float mn = mean_int(a, l);
  
  /* First pass: sum positive and negative delta means for redistribution */    
  uint pdm = 0;
  float ndm = 0;  
  ulong i;
  float d;
  for (i=0; i<l; i++){    
    d = *(a+i) - mn;
    if (d > 0){
      pdm += floor(d); /* Accumulate integer part only for positive delta means: actually available units */
    }else if (d < 0){
      ndm += -d;
    }
  }
   
  uint pool = round(push*pdm);    
  
  /* Now we need to calc the fraction set. */
  /* That should be on the basis of the pool: actually availble material for redistribution. */
  /* Second pass: calc fraction sets of positive and negative delta means. */
  float *pdm_fs = (float *)malloc(sizeof(float)*l);
  float *ndm_fs = (float *)malloc(sizeof(float)*l);
  
  for (i=0; i<l; i++){
    d = *(a+i) - mn;
    if (d > 0){
      *(pdm_fs+i) = (pdm > 0) ? (floor(d)/pdm) : (0);
      *(ndm_fs+i) = 0;
    }else if (d < 0){
      *(ndm_fs+i) = (ndm > 0) ? (-d/ndm) : (0);
      *(pdm_fs+i) = 0;
    }else
      *(pdm_fs+i) = *(ndm_fs+i) = 0;
  }
  
  /* Third pass: advance through fracset, rounding cuts, and re-calcing fracset */  
  uint cut;
  uint take = pool;
  i = 0;
  while (i<l){
    while (i<l && *(pdm_fs+i)==0){
      *(o+i) = *(a+i);
      i++;
    }
    if (*(pdm_fs+i)){
      cut = round(*(pdm_fs+i)*take);
      *(o+i) = *(a+i) - cut;
      take -= cut;
      *(pdm_fs+i) = 0;
      recalc_fracset(pdm_fs, l);
    }
    i++;
  }
  
  /* Fourth pass: redistribute pool */
  i = 0;
  while (i<l){
    while (i<l && *(ndm_fs+i)==0){
      i++;
    }
    if (*(ndm_fs+i)){
      cut = round(*(ndm_fs+i)*pool);
      *(o+i) += cut;
      pool -= cut;
      *(ndm_fs+i) = 0;
      recalc_fracset(ndm_fs, l);
    }
    i++;
  }
  
  free(pdm_fs);
  /* free(ndm_fs); */
  return o;
}
