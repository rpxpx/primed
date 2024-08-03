
/* Number theory
 * Functions for working with sequences of integers: primes and composites.
 * Includes routines for generating Ulam spirals: standard binary and composite color gradient.
 * Work in progress. Routines are split between this file and distributions.h.
 * https://github.com/rpxpx/
 * https://clearlight.systems/
 */ 


#ifndef _PRIMED_H_
#define _PRIMED_H_

/* includes */
/* constants */
/* macros */
/* structs,typedefs */
/* declarations */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> /* memset() */
#include <math.h>
#include <time.h>

#include "distributions.h"
#include "colorspace.h"


/* Basis of everything: integer array */
struct inta_struct{
  int *a;
  ulong l;
};
typedef struct inta_struct inta;

#define INTA_NULL {NULL, 0}

#define IS_ODD(n) ((n) & 1)
#define IS_EVEN(n) (!IS_ODD(n))

int min(int a, int b);
int max(int a, int b);

/* Boolean arrays; basis for standard sieve of Eratosthenes */
void null_boola(bool *a, ulong l);
void fill_boola(bool *a, ulong l, bool v);
ulong count_T(bool *a, ulong l);
void print_bool(bool *a, ulong l);
/* Take Boolean array a to be an l-length code for a strictly increasing integer sequence from (start) */
void print_ints(bool *a, long start, ulong l);

ulong count_nonzero(int *a, ulong l);
/* Expand a base-exponent (bn) integer pair into an array of powers for exponent values from 1 to n */
void expand_bn(int *exp, int *bn);

/* integer array basic manipulation */
void null_inta(inta a);
void fill_inta(inta a, int v);
void print_inta(inta a);
void print_inta_slice(inta a, ulong s, ulong e);
int prep_inta(inta *a, ulong l);
int copy_inta(inta a, inta b);
int wipe_inta(inta *a);
inta clone_inta(inta a);
/* Return slice of (a) from s to e, excluding e, as new inta. */
inta slice_inta(inta a, ulong s, ulong e);
void add_inta(inta a, int n); /* mutative */

int rndm_one(int lo, int hi);
inta rndm_inta(ulong l, int lo, int hi);
inta insert_inta(inta a, int n, ulong p);
inta append_inta(inta a, int n);
inta prepend_inta(inta a, int n);
inta concat_inta(inta a, inta b);

/* Indices of all n in a */
ulong *whereis(inta a, int n);
/* Indices of min, max */
ulong min_inta(inta a);
ulong max_inta(inta a);
ulong *minmax_inta(inta a);
/* Indices of n smallest elements in a */
ulong *mins_inta(inta a, ulong n);
long sum(inta a);
float mean(inta a);
float median(inta a);
inta modes(inta a);
float variance(inta a, uint ddof);
float std_dev(inta a, uint ddof);

/* successively add elements of (a) */
inta accumulate(inta a);
/* return diffs a-b: subtract vals of inta b from inta a */
inta diffs(inta a, inta b);
inta steps(inta a, bool up);
/* a metric of monotonicity: fraction of summed step rises over summed step falls. */
float monotonicity(inta a);
/* squash a discrete curve down with push [0,1] into a more uniform distribution, returning a new array. */
inta uniformize(inta a, float push);
/* return new inta with all unique values from a, in order of occurance. */
inta uniques(inta a);
inta uniques_(inta a);
/* count occurances of each element of b in a */
inta occurance_count(inta a, inta b);
/* outer product of two integer arrays */
inta outer_prod(inta a, inta b);
/* outer product for >2 int arrays. Work RHS->LHS */
inta outer_prod_many(inta *a, ulong l);

/* Standard sieve of Eratosthenes. */
bool *primes_bool(ulong n);
/* Prime-counting function, utilizing Eratosthenes */
ulong pi(ulong n);
ulong pi_approx_Gauss(ulong n);
/* All primes <= n */
inta primes(ulong n);

/* Count zero-remainder divisions of n by d. */
uint div_count(ulong n, ulong d);
/* Prime decomposition (factorization) of natural n.*/
/* Return inta of prime/exponent pairs. e.g. (2600) --> [2,3,5,2,13,1] */
inta decompose(ulong n);
/* All divisors of natural n */
inta divizors(ulong n);
/* Enhanced Sieve of Eratosthenes */
inta compositionality(ulong n);

/* Functions for mapping square spiral location to a sequence index */
int grid_ring(uint x, uint y, uint s);
long grid_spiral_indx(uint x, uint y, uint s);

int ulam_spiral_ascii(uint s, bool wht_on_blk);
int ulam_spiral(FILE *fp, uint s, bool wht_on_blk);

int ulam_spiral_composites_ascii(uint s);
int ulam_spiral_composites(FILE *fp, rgb (*cmap)(float n), float push, uint s, uint cplot, FILE *lfp, uint ls);

/* Sorting */
int sort_inta(inta a);
int swap_two(int *a, int *b);
int sort_two(int *a, int *b);
int sort_three(int *a, int *b, int *c);
int quicksort_hoare(inta a, uint depth);

#endif /* _PRIMED_H_ */
