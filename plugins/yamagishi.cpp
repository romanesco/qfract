/*
 * Yamagishi map(?) 
 * Newton's method for F(x,y) = (x+xy, y^2-x^2) 
 * in R^2
 */

#include "../plugin.h"
using namespace QFract;

#define FUNCTION \
  den = 2*(y+nor); \
  tmp = x*nor/den; \
  y = (y*y-x*x+y*nor)/den; \
  x = tmp;

const char* NAME = "2-dim Newton's method with indeterminate point";
const char* CHILD = "";
const char* COLORMAP = "default.map";

const double XL = -1.5;
const double YT = 1.5;
const double XR = 1.5;
const double YB = -1.5;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM;

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double x=z.x;
  register double y=z.y;
  double nor = x*x+y*y, tmp, tmp2, den;
  int closeToZero=max+1;
    
  register int i;
  for ( i=0; i<max; i++ ) {
    FUNCTION;
    nor = x*x+y*y;
    if (nor < 0.0001) {
      closeToZero = (i < closeToZero) ? i : closeToZero;
    } else {
      closeToZero = max+1;
    }

    // other roots
    tmp = y+1;
    tmp *= tmp;
    tmp2 = x-1;
    tmp2 *= tmp2;
    if ( tmp+tmp2 < 0.01 ) {
      return i*2;
    }
    tmp2 = x+1;
    tmp2 *= tmp2;
    if ( tmp+tmp2 < 0.01 ) {
      return i*2+64;
    }
  }

  if (closeToZero <= max) {
    if (y>0) {
      return closeToZero*2 + 128;
    } else {
      return closeToZero*2 + 192;
    }
  }
  return -1;
}

Point map(Point z, Point c, Parameter param)
{
  register double x=z.x;
  register double y=z.y;
  double nor = x*x+y*y, tmp, tmp2, den;
  
  FUNCTION

  return Point(x,y);
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo * i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP);
  return i;
}
}
