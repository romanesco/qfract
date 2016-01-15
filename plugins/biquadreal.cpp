/*
 * Real biquadratic family
 * z -> (z^2 + a)^2 + b (real parameter space)
 */

#include "../plugin.h"
using namespace QFract;

#define RE2IM2   re2=re*re; \
                 im2=im*im;

#define FUNCTION im = 2*re*im; \
                 re = re2-im2 + a; \
				 RE2IM2 \
				 im = 2*re*im; \
                 re = re2-im2 + b;

#define INCOND  ( (re2+im2)<10 )

const char* NAME = "Swallow (real biquadratic maps)";
const char* CHILD = "biquadrealjulia.so";

const double XL = -2.0;
const double YT = 2.0;
const double XR = 2.0;
const double YB = -2.0;
const double CRE = 0.0;
const double CIM = 1.0;

const int MAXITER = 100;
const int MAXORBIT = 10;
const Parameter PARAM(0);

extern "C" {
int iter(Point z, Parameter param, int max)
{
  register double re=0.0;
  register double im=0.0;
  register double a=z.x;
  register double b=z.y;
  register double re2=re*re;
  register double im2=im*im;

  register int i;
  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  int j=i;

  re=z.y; im=0.0;
  re2=re*re; im2=im*im;

  for ( i=0; (i<max) && INCOND; i++ ) {
    FUNCTION
    RE2IM2
  }

  if ((i>=max) && (j>=max))
    return -1;
  else 
    return ((i<j) ? j: i+128);
}

Point map(Point z, Point c, Parameter param)
{
  register double re=z.x;
  register double im=z.y;
  register double a=c.x;
  register double b=c.y;
  register double re2=re*re;
  register double im2=im*im;

  FUNCTION

  return Point(re,im);
}

Point init(Point c, Parameter param) {return Point(c.y,0);}

PluginInfo* getInfo()
{
  PluginInfo* i= new PluginInfo( NAME, XL, YT, XR, YB, MAXITER, MAXORBIT,
				 PARAM, CHILD);
  return i;
}
}
