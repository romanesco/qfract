/*
 * Mandelbrot set with period 2 external rays
 */

#include "../plugin.h"
using namespace QFract;
#include <cmath>

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define FUNCTION im = 2*re*im + cim; \
                 re = re2-im2 + cre;

#define INCOND  ( (re2+im2)<100 )

const char* NAME = "Quadratic Family (parameter space with period 2 rays)";
const char* CHILD = "quadrayper2julia.so";
const char* COLORMAP = "per2.map";

const double XL = -2.5;
const double YT = 2.0;
const double XR = 1.5;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

const double slope=-sqrt(3);
//const double slope=-tan(M_PI*1/3);
//const double slope=-1.732050808;

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double re=z.x;
  register double im=z.y;
  register double cre=re;
  register double cim=im;
  register double re2=re*re;
  register double im2=im*im;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if (i<max) {
    if ( (slope*re > im) && (slope*re > -im) )
      return i*3+1;
    return i*3;
  }
  else
    return -1;
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double cre=c.x;
  register double cim=c.y;
  register double re2=re*re;
  register double im2=im*im;

  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param) {return c;}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD, COLORMAP);
  return i;
}
}
