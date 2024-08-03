
#include "primed.h"


int min(int a, int b){
  return (b<a)?(b):(a);
}

int max(int a, int b){
  return (b>a)?(b):(a);
}


/* Boolean arrays; basis for standard sieve of Eratosthenes */
void null_boola(bool *a, ulong l){
  memset(a, 0, sizeof(bool)*l);
  return;
}

void fill_boola(bool *a, ulong l, bool v){
  while (l--)
    *(a+l) = v;
  return;
}

ulong count_T(bool *a, ulong l){
  ulong c = 0;
  while (l--)
    if (*(a+l))
      c++; 
  return c;
}

/* Debugging */
void print_bool(bool *a, ulong l){
  ulong i;
  for (i=0; i<l; i++)
    printf("%d ", *(a+i));
  printf("\n");
  return;
}

/* Take Boolean array (a) to be an l-length code for a strictly increasing integer sequence from (start) */
void print_ints(bool *a, long start, ulong l){
  ulong i=0; 
  for (; i<l; i++)
    if (*(a+i))
      printf("%ld ",i+start);
  printf("\n");
  return;
}

/* */


ulong count_nonzero(int *a, ulong l){
  ulong c = 0;
  while (l--)
    if (*(a + l))
      c++; 
  return c;
}

/* Expand a base-exponent (bn) integer pair into an array of powers for exponent values from 1 to n */
/* e.g. [2,6] --> [2,4,8,16,32,64] */
/* Assumes memory in (exp) sufficient */
void expand_bn(int *exp, int *bn){
  uint i;
  for (i=0; i<*(bn+1); i++)
    *(exp+i) = pow(*(bn+0), i+1); 
  return;
}


/* integer array basic manipulation */
/* */
void null_inta(inta a){
  memset(a.a, 0, sizeof(int)*a.l);
  return;  
}

void fill_inta(inta a, int v){
  while (a.l--)
    *(a.a+a.l) = v;
  return;
}

void print_inta(inta a){
  if (a.l==0){
    printf("Null inta\n");
    return;
  }
  ulong i;
  for (i=0;i<a.l;i++)
    printf(" %d",*(a.a+i));
  printf("\n");
  return;
}

void print_inta_slice(inta a, ulong s, ulong e){
  if (a.l==0){
    printf("Null inta\n");
    return;
  }
  if (s > e)
    s = 0;
  if (e > a.l)
    e = a.l;
  if (s)
    printf("...");
  while (s < e)
    printf(" %d", *(a.a+s++));
  if (e < a.l)
    printf("...");
  printf("\n");
  return;
}

/* Copy a to b, into mem at passed pointer */
/* Assume correct size allocated, and b.l correctly set */
int copy_inta(inta a, inta b){
  if (a.l<1)
    return 1;
  while (a.l--)
    *(b.a+a.l) = *(a.a+a.l);
  return 0;
}

int prep_inta(inta *a, ulong l){
  if (l==0){
    (*a).a = NULL;
    (*a).l = 0;
  }else{
    (*a).a = malloc(sizeof(int)*l);
    (*a).l = l;
  }
  return 0;
}

int wipe_inta(inta *a){
  if ((*a).l<1){
    return 1;
  }
  free((*a).a);
  (*a).a = NULL;
  (*a).l = 0;
  return 0;
}

inta clone_inta(inta a){
  if (a.l==0)
    return a;

  inta o;
  prep_inta(&o,a.l);
  copy_inta(a,o);
  return o;
 }

/* Return slice of (a) from s to e as new inta. */
inta slice_inta(inta a, ulong s, ulong e){
  inta o = INTA_NULL;
  if (!(s<a.l && s<e))
    return o;
  e = min(a.l, e); /* Error correction: set e to max value of a.l */
  prep_inta(&o, e-s);  
  while (s<e){
    *(o.a+(o.l-e+s)) = *(a.a+s);
    s++;
  }
  return o;
}

/* Mutative */
void add_inta(inta a, int n){ 
  add_int(a.a, a.l, n);
  return;
}


int rndm_one(int lo, int hi){
  long s = hi-lo+1; /* Range inclusive.*/
  if (s>RAND_MAX)
    return lo-1;
  srand((unsigned)time(NULL)); /* Seed with unixtime. */
  return rand()%s+lo;
}

inta rndm_inta(ulong l, int lo, int hi){
  inta o = INTA_NULL;
  long s = hi-lo+1; /* Range inclusive. */
  if (l<1 || s>RAND_MAX)
    return o;

  srand((unsigned)time(NULL)); /* Seed rand with unixtime. */
  prep_inta(&o,l);
  do
    *(o.a + --l) = rand()%s+lo;
  while (l);
  return o;
}

inta insert_inta(inta a, int n, ulong p){
  if (p>a.l)
    p = a.l;
  inta o;
  o.l = a.l+1;
  o.a = malloc(sizeof(int)*o.l);
  ulong i;
  for (i=0;i<p;i++)
    *(o.a+i) = *(a.a+i);
  *(o.a+i) = n;
  for (;i<a.l;i++)
    *(o.a+i+1) = *(a.a+i);
  return o;
}

inta append_inta(inta a, int n){
  return insert_inta(a,n,a.l);
}

inta prepend_inta(inta a, int n){
  return insert_inta(a,n,0);
}

inta concat_inta(inta a, inta b){
  inta out = INTA_NULL;
  return out;
}



/* Indices of all n in a */
ulong *whereis(inta a, int n){
  ulong *ind;
  if (a.l==0){
    ind = prep_index(0);
    return ind;
  }  
  /* Load n into an inta to pass through occurance_count() */
  inta n_;
  prep_inta(&n_, 1); *(n_.a) = n;
  inta n_count = occurance_count(a, n_);
  int n_c = *(n_count.a);
  wipe_inta(&n_); wipe_inta(&n_count);
  
  ind = prep_index(n_c);
  
  while (a.l--)
    if (*(a.a+a.l)==n)
      *(ind + --n_c) = a.l;
  return ind;
}

/* Index of min */
ulong min_inta(inta a){
  if (a.l==0)
    return UL_END;
  
  ulong i = a.l-1;
  while(a.l--)
    if (*(a.a+a.l) < *(a.a+i))
      i = a.l;
  return i;
}

/* Index of max */
ulong max_inta(inta a){
  if (a.l==0)
    return UL_END;
  
  ulong i = a.l-1;  
  while (a.l--)
    if (*(a.a+a.l) > *(a.a+i))
      i = a.l;
  return i;
}

/* Indices min, max */
ulong *minmax_inta(inta a){
  if (a.l==0)
    return NULL;
  
  ulong *ii = (ulong *)malloc(sizeof(ulong)*2);
  *(ii+0) = *(ii+1) = --a.l;
  
  while (a.l--){
    if (*(a.a+a.l) < *(a.a+*(ii+0)))
      *(ii+0) = a.l;
    else if (*(a.a+a.l) > *(a.a+*(ii+1)))
      *(ii+1) = a.l;
  }
  return ii;
}

/* Indices of n smallest elements in (a) */
/* Calls on type-generic fucntion in distributions.h */
ulong *mins_inta(inta a, ulong n){
  return mins_int(a.a, a.l, n);
}


long sum(inta a){
  long o = 0;
  while(a.l--){
    o += *(a.a+a.l);
  }
  return o;
}

float mean(inta a){
  return (float)sum(a)/a.l;
}

float median(inta a){
  if (a.l==0)
    return 0;
  inta b = clone_inta(a);
  quicksort_hoare(b, 0);
  print_inta(b);
  float o;
  if (IS_ODD(b.l))    
    o = *(b.a+(int)(b.l/2));
  else
    o = (*(b.a+(b.l/2)) + *(b.a+(b.l/2-1)))/2.0;
  wipe_inta(&b);
  return o;
}


inta modes(inta a){
  inta out = INTA_NULL;
  
  inta unqs = uniques(a);
  inta freq = occurance_count(a, unqs);
  ulong max_i = max_inta(freq);
  ulong min_i = min_inta(freq);
  
  /* If (freq) is empty, or contains no mode: max==min, all frequency vals same, return null inta. */
  if (max_i==UL_END || *(freq.a+max_i)==*(freq.a+min_i))
    return out;
  
  ulong *maxima = whereis(freq, *(freq.a+max_i));

  /* Determine number of modes and prep (out) to take them. */
  ulong i;
  for (i=0; *(maxima+i)!=UL_END; i++)
    ;
  prep_inta(&out, i);
  /* Load the modes into (out) applying the indices of max occurances to set of uniques */
  while (i--)
    *(out.a+i) = *(unqs.a + *(maxima+i));
  
  free(maxima);
  return out;
}


/* By convention, reduce degrees of freedom by 1 for sample data */
/* ddof: delta degrees of freedom */
float variance(inta a, uint ddof){
  float xbar = mean(a);
  float sumdiffs = 0;
  ulong i;
  for (i=0;i<a.l;i++)
    sumdiffs += pow((*(a.a+i)-xbar),2);
  return sumdiffs/(a.l-ddof); 
}

float std_dev(inta a, uint ddof){
  return sqrt(variance(a, ddof));
}



/* successively add elements of (a) */
inta accumulate(inta a){
  if (a.l==0)
    return a;
  
  inta out;
  prep_inta(&out, a.l);
  ulong i=0;
  *(out.a+0) = *(a.a+0);
  for (i=1;i<a.l;i++)
    *(out.a+i) = *(a.a+i)+*(out.a+i-1);
  return out;
}


/* Return diffs a-b: subtract vals of inta b from inta (a) */
inta diffs(inta a, inta b){
  inta out = INTA_NULL;
  if (a.l==0 || !(a.l==b.l))
    return out;
  prep_inta(&out, a.l);
  while (a.l--)
    *(out.a+a.l) = *(a.a+a.l)-*(b.a+a.l);
  return out;
}

/* If up==true, assume array is increasing, returning +ve ints for increases; */
/* If up==false, assume array is decreasing, returning -ve ints for increases; */
inta steps(inta a, bool up){
  if (a.l==0)
    return a;
  
  inta out;
  prep_inta(&out, a.l);
  *(out.a+0) = 0;
  inta b;
  b.a = a.a+1;
  b.l = --a.l;

  if (up){
    while (a.l--)
      *(out.a+a.l+1) = *(b.a+a.l)-*(a.a+a.l);
  }else{
    while (a.l--)
      *(out.a+a.l+1) = *(a.a+a.l)-*(b.a+a.l);
  }
  return out;
}


/* Return a measure of monotonicity: Fraction of summed step rises over summed step falls. */
/* +1 for perfect monotonicity; -1 for perfect anti-monotonicty; 0 for equal values */
float monotonicity(inta a){
  inta b,c;
  b.a = a.a+1;
  b.l = --a.l;
  c = diffs(b,a);
  /* Sum +ve and -ve values separately. */
  float ups,dns;
  ups = dns = 0;

  while(c.l--){
    if (*(c.a+c.l)>0)
      ups += *(c.a+c.l);
    if (*(c.a+c.l)<0)
      dns -= *(c.a+c.l);
  }
  
  if (ups>dns)
    return 1-dns/ups;
  if (dns>ups)
    return ups/dns-1;
  return 0; 
}

/* Squash a discrete curve down with push [0,1] into a more uniform distribution, returning a new array. */
inta uniformize(inta a, float push){
  inta o;
  o.l = a.l;
  o.a = uniformize_int(a.a, a.l, push);
  return o;
}


/* return new inta with all unique values from a, in order of occurance. */
/* Most obvious algorithm, brute force: scans cache for each instance, expands cache on discovery of new unique value. */
inta uniques(inta a){
  inta o = INTA_NULL;
  if (a.l == 0)
    return o;

  inta tmp;
  ulong i,j;
  bool seen;
  for (i=0; i<a.l; i++){
    seen = false;
    for (j=0; !seen && j<o.l; j++)
      if (*(o.a+j)==*(a.a+i))
	seen = true;      
    if (!seen){
      tmp = append_inta(o, *(a.a+i));
      wipe_inta(&o);
      o = tmp;
    }
  }
  return o;
}


/* Return a new inta with all unique values from a, in order of occurance. */
/* Alternative algorithm, not brute force. */
/* I conject, faster for very heterogenous sets, slower for homogenous sets. */
/* Compositionality grades for natural numbers are obviously very homogenous. */
inta uniques_(inta a){  
  inta o = INTA_NULL;
  if (a.l == 0)
    return o;

  /* First pass. Make a copy to destroy, & record location of first zero. */
  /* (After this pass, use zero as a marker.) */
  inta b;
  prep_inta(&b, a.l);
  
  ulong z = UL_END;
  while (a.l--){
    if (*(a.a+a.l)==0)
      z = a.l;
    *(b.a+a.l) = *(a.a+a.l);
  }
  
  /* Second pass. Advance to each successive non-zero; shoot forward from there, nulling every repeat. */
  ulong i,j;
  for (i=0; ;i++){    
    while (i<b.l && *(b.a+i)==0)
      i++;
    /* Break if hit the end */
    if (i>=b.l)
      break;
    /* Shoot forward, nulling every repeat. */
    j = i+1;
    while (j<b.l){
      if (*(b.a+j)==*(b.a+i))
	*(b.a+j) = 0;
      j++;
    }
  }
  
  /* Third pass. Count all non-zeros, add z. Prepare inta to contain them for return. */
  ulong nz = 0;
  for (i=0; i<b.l; i++)
    if (*(b.a+i) != 0)
      nz++;
  prep_inta(&o, nz + ((z==UL_END)?(0):(1)) );
  
  /* Fourth pass. Add all zero if z and all non-zero elements of b. */
  j = 0;
    for (i=0; i<b.l; i++){
    if (*(b.a+i) != 0)
      *(o.a+j++) = *(b.a+i);
    else if (i==z)
      *(o.a+j++) = 0;
  }
    
  wipe_inta(&b);  
  return o;
}


/* Count occurances of each element of b in a */
inta occurance_count(inta a, inta b){
  inta o = INTA_NULL;
  if (a.l==0 || b.l==0){    
    return o;
  }
  
  prep_inta(&o, b.l);
  null_inta(o);
  
  ulong i;
  while (b.l--)
    for (i=0; i<a.l; i++)
      if (*(a.a+i)==*(b.a+b.l))
	(*(o.a+b.l))++;
  return o;
}


/* outer product of two int arrays */
inta outer_prod(inta a, inta b){
  inta o = INTA_NULL;
  if (a.l<1 || b.l<1)
    return o;
  
  o.l = a.l*b.l;
  o.a = malloc(sizeof(int)*o.l);
  ulong i,j;
  for (i=0;i<a.l;i++)
    for (j=0;j<b.l;j++)
      *(o.a+(i*b.l + j)) = *(a.a+i) * *(b.a+j);  
  return o;
}

/* outer product for >2 int arrays. Work RHS->LHS */
inta outer_prod_many(inta *a, ulong l){
  inta o = INTA_NULL;

  /* Trivial cases */
  if (l==0 || a[0].l==0)
    return o;
  if (l==1){
    o = clone_inta(a[0]); 
    return o;
  }
  l--;
  inta t = clone_inta(*(a+l));
  while(l--){
    o = outer_prod(*(a+l), t); 
    free(t.a);
    t = o;
  }
  return o;
}


/* Standard sieve of Eratosthenes. */
/* Find all primes up to n. */
/* Return Boolean array coding primes=1 and non-primes=0 for all naturals <= n */
bool *primes_bool(ulong n){
  bool *sieve = NULL;
  
  if (!(sieve = (bool *)malloc(sizeof(bool)*n))){
    fprintf(stderr,"err: malloc(%ld) request failed, primes_bool\n", sizeof(bool)*n);
    return NULL;
  }
  
  fill_boola(sieve, n, 1);
  
  *(sieve+0) = 0; /* By convention, 1 is not prime */
  ulong i,j;
  i = 1; /* ...therefore start the sieve algorithm at 2 (index 1), first prime.*/

  /* Shoot out rays forward from each prime to all multiples, switching to 0. */  
  for (; i<n; i++){
    /* Advance (i) to next prime. */
    for(; *(sieve+i)==0 && i<n; i++)
      ;
    /* Zero all j=>2 multiples of (i) */
    for(j=2; ((i+1)*j)<=n; j++)
      *(sieve + (i+1)*j-1) = 0;
  }
  return sieve;
}


/* Prime-counting function, utilizing Eratosthenes */
ulong pi(ulong n){
  bool *sieve;
  if (!(sieve = primes_bool(n))){
    fprintf(stderr,"err: failed to create sieve to %ld\n", n);
    return 0;
  }
  return count_T(sieve, n);
}

ulong pi_approx_Gauss(ulong n){
  return round(n/log(n));
}

/* All primes <= n */
/* Translate prime/non-prime boolean array into inta of primes only.*/
inta primes(ulong n){
  inta o = INTA_NULL;  

  bool *sieve;
  if (!(sieve = primes_bool(n))){
    fprintf(stderr,"err: failed to create sieve to %ld\n", n);
    return o;
  }
  
  /* Length of int array == number of T values in bool array */
  ulong l = count_T(sieve, n);
  
  /* Convert Boolean array into integer array.*/
  int *prms;
  if (!(prms = (int *)malloc(sizeof(int)*l))){
    fprintf(stderr,"err: failed to create %ld-length inta for primes\n", l);
    return o;
  }
  
  ulong i,j;
  for (i=j=0; i<n; i++)
    if (*(sieve+i))
      *(prms + j++) = i+1;
  
  free(sieve);  
  o.a = prms; o.l = l;  
  return o;
}


uint div_count(ulong n, ulong d){
  if (d < 2)
    return 0;
  uint k = 0;
  while (n%d==0){
    n /= d;
    k++;
  }
  return k;
}


/* Prime decomposition (factorization) of natural n.*/
/* Return inta of prime/exponent pairs. e.g. (2600) --> [2,3,5,2,13,1] */
inta decompose(ulong n){
  
  /* Create set of all primes <=n */
  inta p = primes(n);
  /* Initialize base-exponent array to null.*/
  inta bn = INTA_NULL;
  if (p.l==0)
   return bn;
  
  /* Check for trivial solution first: n is prime; n is final integer in the prime set, p.*/
  if (n==*(p.a+p.l-1)){
    bn.a = (int *)malloc(sizeof(int)*2);
    *(bn.a) = n;
    *(bn.a+1) = 1;
    bn.l = 2;
    return bn;
  }

  /* Non-trivial: n is not prime; n is a product of exponentiated primes.*/
  /* Find exponents; count nonzero exponents.*/
  int *exp = (int *)malloc(sizeof(int)*p.l); /* array for exponents of primes */
  int k = 0; /* counter for nonzero exponents */
  int i = 0;
  for (i=0; n>1; i++){
    *(exp+i) = 0;
    while (n%*(p.a+i)==0){
      *(exp+i) += 1;
      n /= *(p.a+i);
    }
    if (*(exp+i))
      k++;
  }
  /* i is now the length of exponent array. */
  /* k is the number of nonzero exponents. */

  /* Splice primes and exponents into a single array for return.*/
  bn.a = (int *)malloc(sizeof(int)*2*k);

  bn.l = 2*k;
  int j,m;
  for(j=m=0; j<i; j++){
    if (*(exp+j)){
      *(bn.a+2*m) = *(p.a+j);
      *(bn.a+(2*m+1)) = *(exp+j);
      m++;
    }
  }
 
  free(p.a);
  free(exp);
  return bn;
}


/* All divisors of natural n */
/* calculated with prime decomposition of n */
/* Returned unsorted; not increasing. */
inta divizors(ulong n){
  
  /* First find prime decomposition of n.*/
  inta bn = decompose(n);
  
  /* Create an array of intas, length of bn decomp/2.  */
  /* Expand bn decomp into this. */
  /* Prepend 1s. Pass to outer_prod_many(). */
  /* Set of all divisors is just this outer product. */
  
  /* Expand bn pairs. */
  inta expnd[bn.l/2]; /* bn.l/2 is the number of unique primes. */
  uint i;
  for (i=0;i<bn.l/2;i++){
    expnd[i].l = *(bn.a+2*i+1)+1; /* +1 for prepended 1 */
    expnd[i].a = malloc(sizeof(int)*expnd[i].l); 
    *(expnd[i].a) = 1;
    expand_bn((expnd[i].a+1), (bn.a+2*i));
  }
  
  inta o = INTA_NULL;
  o = outer_prod_many(expnd, bn.l/2);
  
  /* Free up mem in expnd before exiting. */
  for (i=0;i<bn.l/2;i++)
    free(expnd[i].a);

  return o;
}



/* Enhanced Sieve of Eratosthenes */
/* - primes marked 1 */
/* - composites set to degree of compositionality: number of factors in their prime decomposition */
/* - 1 conventionally non-prime and non-composite, marked 0 */
inta compositionality(ulong n){
  inta o = INTA_NULL;
  if (n<1)
    return o;
  
  if (!(o.a = (int *)malloc(sizeof(int)*n))){
    fprintf(stderr, "err: malloc(%ld) request failed, in compositionality()\n", sizeof(int)*n);
    return o;
  }
  o.l = n;
  null_inta(o); /* Set initial composition value 0 for all numbers. */
  
  ulong i,j,nij;
  i = 1; /* Start the algorithm at 2, first prime.*/
  
  for (; i<n; i++){
    /* Advance to next prime.*/
    for(; *(o.a+i)!=0 && i<n; i++)
      ;    
    /* Shoot out rays forward from this prime to all multiples, beginning with 2. */
    j = 2;        
    nij = (i+1)*j;    
    while (nij<=n){
      *(o.a + nij-1) += div_count(nij, i+1);
      nij = (i+1)*++j;
    }
  }
  /* Final pass, set all 0s, marking primes, to 1.*/
  for (i=1; i<n; i++)
    if (*(o.a+i)==0)
      *(o.a+i) = 1;
  return o;
}



/* Ulam spirals */

/* Return grid ring from x, y coordinates (0,0 at upper left) and total square size.*/
int grid_ring(uint x, uint y, uint s){
  if (!(x < s) || !(y < s))
    return -1; /* Input error */
  s--; /* Convert square size into largest coordinate number.*/
  return max(max(s-2*x, 2*x-s), max(s-2*y, 2*y-s)) + 1;
}

/* Assume spiral goes anti-clockwise, and starts from bottom-right corner of grid;
 * odd-size squares: starts centre, moves right; 
 * even-sized squares: starts TL, moves left. 
 * Change rotational direction and start point after by applying mirror/rotate to bitmap.
 */
long grid_spiral_indx(uint x, uint y, uint s){  
  int r = grid_ring(x, y, s);
  if (r < 1)
    return -1;
  if (r == 1)
    return 0;
  
  long i; /* spiral index */
  
  /* Translate origin to top left of ring */
  x -= (s-r)/2;
  y -= (s-r)/2;
  
  /* Work around sides of the spiral, adding steps as applicable */
  if (x==(r-1))          /* R */
    if (y==(r-1))        /* BR, final point, special case */
      i = 4*(r-1);
    else
      i = (r-1)-y; 
  else if (y==0)         /* T */
    i = (r-1)+(r-1-x);
  else if (x==0)         /* L */
    i = 2*(r-1) + y;
  else                   /* B <BR */
    i = 3*(r-1) + x;
  return (r-2)*(r-2) - 1+i;
}


/* Print (s x s) standard Ulam spiral to STDOUT as ascii pbm.*/
int ulam_spiral_ascii(uint s, bool prime_code){
  if (s==0)
    return 1;
  
  bool *prms = primes_bool(s*s);
  
  printf("P1\n");
  printf("# https://clearlight.systems\n");
  printf("# https://github.com/rpxpx/primed\n");
  printf("# --------------------------------#\n");
  printf("# Ulam spiral, size %dx%d\n",s,s);
  printf("%d %d\n",s,s);
  
  uint x,y;
  for (y=0; y<s; y++){
    for (x=0; x<s; x++)
      printf("%d ", *(prms + grid_spiral_indx(x, y, s)) ^ !prime_code); /* XOR to flip bit */
    printf("\n");
  }
  return 0;
}



/* Write out s x s Ulam spiral to file as binary pbm.*/
int ulam_spiral(FILE *fp, uint s, bool wht_on_blk){
  if (s==0)
    return 1;
  
  bool *prms = primes_bool(s*s);

  fprintf(fp,"P4\n");
  fprintf(fp,"# https://clearlight.systems\n");
  fprintf(fp,"# https://github.com/rpxpx/primed\n");
  fprintf(fp,"# --------------------------------#\n");
  fprintf(fp,"# Ulam spiral, size %dx%d\n",s,s);
  fprintf(fp,"%d %d\n",s,s);
  
  uint x,y;
  uchar b, b_i, b_l;
  /* Set bitmask as 8 1s or 8 0s.*/
  char mask = 0-wht_on_blk;
  
  for (y=0; y<s; y++)
    for (x=0; x<s; x+=(8-b_l)){
      b = 0; /* Reset byte */
      b_l = 8 - min(s-x, 8); /* Length short of a whole byte.*/
      
      /* Build byte by logical OR-ing L-shfited bits from boolean primes array.*/
      for (b_i=8; b_i>b_l; b_i--)
	b |= (*(prms + grid_spiral_indx(x+8-b_i, y, s))) << (b_i-1);
             
      fprintf(fp, "%c", b ^ mask); /* Flip whole byte if black primes on white composites desired.*/
    }
  return 0;
}


/* Print (s x s) compositional Ulam spiral to STDOUT as ascii.*/
int ulam_spiral_composites_ascii(uint s){
  if (s==0)
    return 1;
  
  inta comp = compositionality(s*s);
  /* Most composite composite */
  int n = max_inta(comp);
  /* its compositionality */
  int m = *(comp.a + n);
  /* Number of digits in m. (For terminal formatting.)*/
  int d = digit_count_int(m); 
  int pad;

  printf("# https://clearlight.systems\n");
  printf("# https://github.com/rpxpx/primed\n");
  printf("# --------------------------------#\n");
  printf("# Square spiral compositionality image of natural numbers to %d\n", s*s);
  printf("# Highest composite value is %d; highest such composite is %d.\n", m, n+1);  
  uint x,y,i;
  int c;  
  for (y=0; y<s; y++){
    for (x=0; x<s; x++){
      c = *(comp.a + grid_spiral_indx(x,y,s));
      pad = d - digit_count_int(c);
      for (i=0; i<pad; i++)
	printf(" ");
      printf(" %d",c);
    }
    printf("\n");
  }
  return 0;
}


/* Colormap of compositionality of natural numbers.*/
/* Write to binary on *fp as ppm.*/
/* Write color legend to *lfp in (ls x ls) pixel sized squares.*/
/* Minimum square size: 2x2: natural numbers 1...4. */

#define BG_GREY 0.7 /* Grey shade (black=1) constant for backgrounding.*/

int ulam_spiral_composites(FILE *fp, rgb (*cmap)(float n), float push, uint s, uint cplot, FILE *lfp, uint ls){
  if (s < 2)
    return 1;
  
  /* */  
  inta comp = compositionality(s*s);
  
  /* Most composite */
  int n = max_inta(comp);
  /* its compositionality */
  int m = *(comp.a + n);
  
  inta unq = uniques(comp);
  inta freq = occurance_count(comp, unq);
  
  /* Slice from 2: first composite has compositionality 2 */
  inta freq_ = slice_inta(freq, 2, freq.l);  
  wipe_inta(&freq); freq = freq_;
  inta freq_u = uniformize(freq, push);

  print_inta(freq);
  print_inta(freq_u);
  

  /* Prepare palette from this array.*/
  inta accum = accumulate(freq_u);
  
  /* Deduct first term from all elements so that array starts at 0 */
  add_inta(accum, -*(accum.a));
  /* Take fractions to make palette */
  float *palette = fractionize_int(accum.a, accum.l, *(accum.a+accum.l-1));

  uint j;
  for (j=0; j < accum.l; j++){
    printf(" %f", *(palette+j));
  }
  printf("\n");
  
  
  /* Free memory */
  wipe_inta(&unq);
  wipe_inta(&freq);  
  wipe_inta(&freq_u);  
  wipe_inta(&accum);  
  
  /* Write spiral */
  fprintf(fp,"P6\n");
  fprintf(fp,"# https://clearlight.systems\n");
  fprintf(fp,"# https://github.com/rpxpx/primed\n");
  fprintf(fp,"# --------------------------------#\n");
  fprintf(fp,"# Enhanced Ulam spiral, size %dx%d\n",s,s);
  fprintf(fp,"# Square spiral compositionality image of natural numbers to %d\n", s*s);
  fprintf(fp,"# Highest composite value is %d; highest such composite is %d.\n", m, n+1);  
  fprintf(fp,"%d %d\n",s,s);
  fprintf(fp,"255\n"); /* Channel state 0-->255: 8 bits.*/
  uint x,y;
  int c;
  rgb p;
  for (y=0; y<s; y++)   
    for (x=0; x<s; x++){
      c = *(comp.a + grid_spiral_indx(x,y,s));
      /* 1: Not prime, nor composite; code grey */
      /* or if not selected compositionality */
      if (c==0 || ((cplot > 0) && (cplot != c)) || cplot > m) 
	p = cmap_achromatic(BG_GREY);
      /* Compositionality 1, prime: code white */
      else if (c==1)
	p = cmap_achromatic(0);
      else
	p = (*cmap)(*(palette+c-2)); /* Minus 2 to exclude compositionality 0 and 1 */            
      fprintf(fp,"%c%c%c", (uchar)(255*p.r), (uchar)(255*p.g), (uchar)(255*p.b));
    }

  /* Write legend */
  fprintf(lfp,"P6\n");
  fprintf(lfp,"# https://clearlight.systems\n");
  fprintf(lfp,"# https://github.com/rpxpx/primed\n");
  fprintf(lfp,"# -------------------------------#\n");
  fprintf(lfp,"# Legend for enhanced Ulam spiral, size %dx%d\n",s,s);
  fprintf(lfp,"# Square spiral compositionality image of natural numbers to %d\n", s*s);
  fprintf(lfp,"# Primes plotted in white.\n");
  fprintf(lfp,"# Colormap coding for composite values 2 to %d\n", m);
  uint i;
  for (i=0; i<(m-1); i++)
    fprintf(lfp,"# %3d\t%f\n",i+2, *(palette+i));
  fprintf(lfp,"%d %d\n",(m-1)*ls, ls);
  fprintf(lfp,"255\n");
  uchar r,g,b;
  for (y=0; y<ls; y++)
    for (i=0; i<(m-1); i++){
      p = (*cmap)(*(palette+i));
      r = 255*p.r; g = 255*p.g; b = 255*p.b;
      for (x=0; x<ls; x++)
	fprintf(lfp,"%c%c%c", r,g,b);
    }    
  return 0;
}



/* SORT ALOGORITHMS */
/* quicksort, et al */
/* Return value is number of swaps. */

/* Default sort algorithm */
int sort_inta(inta a){
  return quicksort_hoare(a, 0);
}

/* Trivial sorts */
/* Units used in complex sorts. */
int swap_two(int *a, int *b){
  int t;
  t = *a; *a = *b; *b = t;
  return 1;
}

int sort_two(int *a, int *b){
  if (*a>*b)
    return swap_two(a, b); 
  return 0;
}

int sort_three(int *a, int *b, int *c){
  int swps = 0;
  swps += sort_two(a, c);
  swps += sort_two(b, c);
  swps += sort_two(a, b);
  return swps;
}


/* QUICKSORT: Median pivot, Hoare partitioning.*/
/* Mutative. Sorts in situ */
/* Recursive */
int quicksort_hoare(inta a, uint depth){
  /* Nothing to do */
  if (a.l<2)
    return 0;
  
  /* Trivial cases */
  if (a.l==2)
    return sort_two(a.a, a.a+1);
  if (a.l==3)
    return sort_three(a.a, a.a+1, a.a+2);  
  
  /* For inta length>3, find pivot and parition. */  
  /* Select pivot value for array. */
  /* Median pivot: pick three values, ends and middle; sort in situ, pick middle value as pivot. */  
  ulong swps;
  ulong lo,hi;
  lo = 0; hi = a.l-1; 
  /* Sort start, end, middpoint. Take middpoint as pivot. Push partition markers inwards. */
  swps = sort_three(a.a+lo++, a.a+a.l/2, a.a+hi--);
  int piv = *(a.a+a.l/2);

  /* Slide inwards, swapping vals around pivot. */
  for (; lo<hi; ){
    if (*(a.a+lo)>=piv){
      if (*(a.a+hi)<piv){
        swps += swap_two(a.a+lo++,a.a+hi--);
      }else{
        hi--;
      }
    }else{
      lo++; 
    }
  }
  
  /* Partition point found; recursively submit non-zero length subpartitions for sorting. */
  /* LH partition: 0-->hi */
  if (hi>0){
    inta b = a;
    b.l = hi+1;
    swps += quicksort_hoare(b, depth+1);
  }
  /* RH partition: lo-->a.l-1 */
  if (lo<a.l-1){
    inta c;
    c.a = a.a+lo;
    c.l = a.l-lo;
    swps += quicksort_hoare(c, depth+1);
  }
  return swps;
}
