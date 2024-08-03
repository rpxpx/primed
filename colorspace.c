
#include "colorspace.h"


/* Remainder of Euclidian division of a by b, where a is possibly not an integer.*/
float mod_float(float a, int b){
  int i = a;
  return i%b + (a-i);  
}

/* r,g,b in [0,1] */
rgb loadrgb(float r, float g, float b){
  rgb o;
  if (r<0 || r>1 || g<0 || g>1 || b<0 || b>1)
    o.r = o.g = o.b = -1;
  else{
    o.r = r;
    o.g = g;
    o.b = b;
  }
  return o;
}

/* h in [0,360], s,l in [0,1] */
hsl loadhsl(float h, float s, float l){
  hsl o;
  if (s<0 || s>1 || l<0 || l>1)
    o.h = o.s = o.l = -1;
  else{  
    o.h = mod_float(h, 360);
    if (o.h<0)
      o.h += 360;
    o.s = s;
    o.l = l;
  }
  return o;
}

/* h in [0,360], s,l in [0,1] */
hsv loadhsv(float h, float s, float v){
  hsl t = loadhsl(h,s,v);
  hsv o = {t.h, t.s, t.l};
  return o;
}



/* RGB colormaps */
rgb cmap_achromatic(float x){
  x = 1-x;
  rgb o = loadrgb(x,x,x);
  return o;
}

/* Maps [0,1] Y-->R-->B. */
rgb cmap_heat(float n){
  rgb o;
  if (n < 0 || n > 1){
    o.r=-1; o.g=-1; o.b=-1;
  }else if (n < (1/3.0)) /* YELLOW */
    o = loadrgb(1, 1-3*n, 0);
  else if (n < (2/3.0)) /* RED */
    o = loadrgb(1, 0, 3*(n-1/3.0));
  else /* 2/3 < n < 1 */ /* BLUE */
    o = loadrgb(3-3*n, 0, 1);
  return o;
}

/* Maps [0,1] R-->B. */
rgb cmap_heat2(float n){
  rgb o;
  if (n < 0 || n > 1){
    o.r=-1; o.g=-1; o.b=-1;
  }else if (n < 0.5) /* RED */
    o = loadrgb(1, 0, 2*n);
  else /* 0.5 < n < 1  BLUE */
    o = loadrgb(2-2*n, 0, 1);  
  return o;
}


/* General HSV colormap
 * Pass 6 floats as coeeficients (slope, intercept) of 3 linear functions for H,S,V, on x in [0,1] */
rgb cmap_hsv(float a, float b, float c, float d, float e, float f, float x){
  hsv col = loadhsv(a*x + b, c*x + d, e*x + f);
  return hsv_to_rgb(col);
}

/* Specific HSV colormaps */
rgb cmap_deepblue(float x){
  return cmap_hsv(0, 260, 0.8, 0.2, -0.4, 1, x);
}

rgb cmap_aqua(float x){
  return cmap_hsv(0, 190, 0.8, 0.2, -0.6, 1, x);
}

rgb cmap_heat_(float x){
  return cmap_hsv(-180, 60, 0, 0.95, 0, 1, x);
}



/* Write an n-band spectrum generated with passed cmap function as an ((n*s) x s) pixel image to .ppm */
void cmap_legend(FILE *fp, rgb (*cmap)(float x), unsigned int s, unsigned int n){
  fprintf(fp,"P6\n");
  fprintf(fp,"# Colormap Legend\n");
  fprintf(fp,"# https://github.com/rpxpx/primed\n");
  fprintf(fp,"# https://clearlight.systems\n");
  fprintf(fp,"%d %d\n", n*s, s); /* width height */
  fprintf(fp,"255\n"); /* channel state 0-->255: 8 bits */

  rgb p;
  uint i,x,y;
  unsigned char r,g,b;    
  for (y=0; y<s; y++)
    for (i=0; i<n; i++){
      p = (*cmap)(i/(n-1.0));
      r = 255*p.r; g = 255*p.g; b = 255*p.b;
      for (x=0; x<s; x++)
	fprintf(fp,"%c%c%c", r,g,b);
    }  
  return;
}



/* Convert an HSL color value to RGB.
 * Conversion formula from http://en.wikipedia.org/wiki/HSL_color_space.
 */
float chroma(hsl col){
  return (1-fabs(2*col.l-1))*col.s;
}

rgb hsl_to_rgb(hsl col){
  rgb o = {-1,-1,-1};
  /* Input error case */
  if (col.s<0 || col.s>1 || col.l<0 || col.l>1)
    return o;

  /* Achromatic case: saturation 0 */
  if (col.s<0.001){
    o.r = o.g = o.b = col.l;
    return o;
  }
  
  col.h = mod_float(col.h, 360);
  if (col.h < 0)
    col.h += 360;
  float hex = col.h/60;
  float chr = chroma(col);
  float x = chr*(1 - fabs(mod_float(hex, 2) - 1));
  float r,g,b;
  if (hex < 1){
    r = chr; g = x; b = 0;
  }else if (hex < 2){
    r = x; g = chr; b = 0;
  }else if (hex < 3){
    r = 0; g = chr; b = x;
  }else if (hex < 4){
    r = 0; g = x; b = chr;
  }else if (hex < 5){
    r = x; g = 0; b = chr;
  }else{ /* hex < 6 */
    r = chr; g = 0; b = x;
  }
  float m = col.l - chr/2;
  o.r = r+m; o.g = g+m; o.b = b+m;  
  return o;  
}

hsl hsv_to_hsl(hsv col){
  hsl o;
  /* H */
  o.h = col.h;
  /* L */
  o.l = col.v*(1-col.s/2);
  /* S : if L~=0 or L~=1, assign 0, else ... */
  o.s = (fabs(round(o.l)-o.l) < 0.0001) ? (0) : ((col.v-o.l)/MIN(o.l, 1-o.l));
  return o;
}

rgb hsv_to_rgb(hsv col){
  hsl t = hsv_to_hsl(col);
  rgb o = hsl_to_rgb(t);
  return o;
}
