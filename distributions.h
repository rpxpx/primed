
/* Discrete distributions
 * Functions for working with integer or float arrays considered as distributions.
 * Work in progress. See also the routines in primed.h.
 * https://github.com/rpxpx/
 * https://clearlight.systems/
 */

#ifndef _DISTRIBUTIONS_H_
#define _DISTRIBUTIONS_H_

/* includes */
/* constants */
/* macros */
/* structs,typedefs */
/* declarations */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h> /* fabs(), pow() */
#include <time.h> /* time() (seeding rand()) */

/* Sometimes we need an array of ulong indices. */
/* We use ulong -1 (all bits set to 1) as terminator value. (This value is unused as an index in inta.) */
#define UL_END -1

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
#define round(f) ((((f)-(long)(f))<0.5) ? ((long)(f)) : ((long)(f)+1))
#define rounddp(f, d) (round(pow(10,(d))*(f)) / pow(10,(d)))


typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

uint digit_count_int(int a);

/* Some functions for working with ulong indices */
ulong *prep_index(ulong l);
void print_index(ulong *ind);
void print_index_full(ulong *ind);
bool is_in_index(ulong *ind, ulong n);

/* Array of floats (x/b) for all x in a */
float *fractionize_int(int *a, ulong l, int b);

/* Let a fracset (fraction set) be an array of floats summing to 1. */
float *gen_fracset(ulong l, uint r);
int recalc_fracset(float *s, ulong l);
float *shrink_fracset(float *s, ulong l, ulong ls);

/* Fundamental functions specific to data type
 * to be passed as pointers to generic functions. */
/* Indexing and comparison */
void *indx_int(void *a, ulong i);
void *indx_float(void *a, ulong i);
int cmp_int(void *n, void *m);
int cmp_float(void *n, void *m);

/* Type-generic functions  */
ulong min_indx(void *(*indx)(void *, ulong), int (*cmp)(void *, void *), void *a, ulong l);
ulong *mins_indx(void *(*indx)(void *, ulong), int (*cmp)(void *, void *), void *a, ulong l, ulong n);

/* Type-specific */
ulong *mins_int(int *a, ulong l, ulong n);
long sum_int(int *a, ulong l);
float mean_int(int *a, ulong l);
void add_int(int *a, ulong l, int n);
ulong *mins_float(float *a, ulong l, ulong n);
float sum_float(float *a, ulong l);

/* Squash a discrete curve down with push [0,1] into a more uniform distribution, returning a new array.*/
/* push must be in interval [0,1]: is the fraction of units above the mean that are transferred to elements below.*/
int *uniformize_int(int *a, ulong l, float push);


#endif /* _DISTRIBUTIONS_H_ */
