
/* Colormaps and color space conversions.
 * Routines for color plots.
 * https://github.com/rpxpx/
 * https://clearlight.systems/
 */



#ifndef _COLORSPACE_H_
#define _COLORSPACE_H_

/* includes */
/* constants */
/* macros */
/* structs,typedefs */
/* declarations */

#include <stdio.h>
#include <math.h> /* fabs() */
#include "distributions.h" /* macros: MIN(), MAX(), ROUND(), ROUNDDP() */


typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;



/* Decimal values in [0,1] */
struct rgb_struct{
  float r;
  float g;
  float b;
};
typedef struct rgb_struct rgb;

struct hsl_struct{
  float h;
  float s;
  float l;
};
typedef struct hsl_struct hsl;

struct hsv_struct{
  float h;
  float s;
  float v;
};
typedef struct hsv_struct hsv;


float mod_float(float a, int b);
rgb loadrgb(float r, float g, float b);
hsl loadhsl(float h, float s, float l);
hsv loadhsv(float h, float s, float v);

/* Map [0,1] W-->B */ 
rgb cmap_achromatic(float x);
/* Map [0,1] Y-->R-->B. */
rgb cmap_heat(float n);
/* Map [0,1] R-->B. */
rgb cmap_heat2(float n);

rgb cmap_hsv(float a, float b, float c, float d, float e, float f, float x);
rgb cmap_deepblue(float x);
rgb cmap_aqua(float x);
rgb cmap_heat_(float x);


/* Write an n-band spectrum generated with passed cmap function as an (n*s)xs pixel image to .ppm */
void cmap_legend(FILE *fp, rgb (*cmap)(float x), unsigned int s, unsigned int n);


/* Convert HSL or HSV color values to RGB.
 * Conversion formulae from http://en.wikipedia.org/wiki/HSL_color_space. */
float chroma(hsl col);
rgb hsl_to_rgb(hsl col);
hsl hsv_to_hsl(hsv col);
rgb hsv_to_rgb(hsv col);


#endif /*_COLORSPACE_H_*/
